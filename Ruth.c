#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/bio.h>
#include <openssl/evp.h>

char* base64_encode(const unsigned char* input, int length) {
    BIO *bio, *b64;
    FILE* stream;
    int encoded_size = 4 * ((length + 2) / 3);
    char* buffer = (char*)malloc(encoded_size + 1);

    stream = fmemopen(buffer, encoded_size + 1, "w");
    b64 = BIO_new(BIO_f_base64());
    bio = BIO_new_fp(stream, BIO_NOCLOSE);https://github.com/Arhoc
    BIO_push(b64, bio);
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO_write(b64, input, length);
    BIO_flush(b64);
    BIO_free_all(b64);
    fclose(stream);

    return buffer;
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Usage: %s <filename> <iterations>\n", argv[0]);
        return 1;
    }

    char* filename = argv[1];
    int iterations = atoi(argv[2]);
    char* obf_pl = NULL;
    char* orig_pl = NULL;

    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return 1;
    }
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);
    orig_pl = (char*)malloc(file_size + 1);
    fread(orig_pl, 1, file_size, file);
    fclose(file);
    orig_pl[file_size] = '\0';

    for (int i = 0; i < iterations; i++) {
        char* v = "base64";
        char* pl_func = NULL;
        char* base = NULL;

        if (strcmp(v, "base64") == 0) {
            int orig_pl_len = strlen(orig_pl);
            char* encoded = base64_encode((const unsigned char*)orig_pl, orig_pl_len);
            base = strdup(encoded);
            free(encoded);

            char* obf_pl_template = "eval(\"use MIME::Base64;eval decode_base64('%s');\")";
            int obf_pl_len = strlen(obf_pl_template) + strlen(base) + 1;
            obf_pl = (char*)malloc(obf_pl_len);
            sprintf(obf_pl, obf_pl_template, base);

            free(orig_pl);
            orig_pl = strdup(obf_pl);
        }

        FILE* f = fopen("fusca.pl", "w");
        if (f == NULL) {
            printf("Error opening file: fusca.pl\n");
            free(obf_pl);
            free(orig_pl);
            return 1;
        }
        fputs(obf_pl, f);
        fclose(f);

        free(obf_pl);
    }

    free(orig_pl);

    return 0;
}
