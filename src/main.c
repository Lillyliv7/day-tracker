#include <stdio.h>

#include <fnvhash.h>
#include <password.h>



int main(int argc, char **argv) {


    printf("%lx\n", fnv1a_hash("helko"));

    if (verify_password("teehee", generate_hash("teehee"))) {
        printf("Password verified successfully.\n");
    } else {
        printf("Password verification failed.\n");
    }


    return 0;
}