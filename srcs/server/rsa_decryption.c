#include "server.h"

void rsa_decrypt(unsigned char *input, unsigned char *output)
{
	FILE *f;
	int ret = 1;
	size_t i;
	mbedtls_rsa_context rsa;
	mbedtls_mpi N, P, Q, D, E, DP, DQ, QP;
	mbedtls_entropy_context entropy;
	mbedtls_ctr_drbg_context ctr_drbg;
	unsigned char result[1024];
	unsigned char buf[512];
	const char *pers = "rsa";

	memset(result, 0, sizeof(result));

	fflush(stdout);

	mbedtls_rsa_init(&rsa, MBEDTLS_RSA_PKCS_V15, 0);
	mbedtls_ctr_drbg_init(&ctr_drbg);
	mbedtls_entropy_init(&entropy);
	mbedtls_mpi_init(&N);
	mbedtls_mpi_init(&P);
	mbedtls_mpi_init(&Q);
	mbedtls_mpi_init(&D);
	mbedtls_mpi_init(&E);
	mbedtls_mpi_init(&DP);
	mbedtls_mpi_init(&DQ);
	mbedtls_mpi_init(&QP);

	ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func,
								&entropy, (const unsigned char *)pers,
								strlen(pers));
	if (ret != 0)
	{
		mbedtls_printf(" failed\n  ! mbedtls_ctr_drbg_seed returned %d\n",
					   ret);
	}

	fflush(stdout);

	if ((f = fopen(PRIVATE_KEY_PATH, "rb")) == NULL)
	{
		mbedtls_printf(" failed\n  ! Could not open rsa_priv.txt\n"
					   "  ! Please run rsa_genkey first\n\n");
	}

	if ((ret = mbedtls_mpi_read_file(&N, 16, f)) != 0 ||
		(ret = mbedtls_mpi_read_file(&E, 16, f)) != 0 ||
		(ret = mbedtls_mpi_read_file(&D, 16, f)) != 0 ||
		(ret = mbedtls_mpi_read_file(&P, 16, f)) != 0 ||
		(ret = mbedtls_mpi_read_file(&Q, 16, f)) != 0 ||
		(ret = mbedtls_mpi_read_file(&DP, 16, f)) != 0 ||
		(ret = mbedtls_mpi_read_file(&DQ, 16, f)) != 0 ||
		(ret = mbedtls_mpi_read_file(&QP, 16, f)) != 0)
	{
		mbedtls_printf(" failed\n  ! mbedtls_mpi_read_file returned %d\n\n",
					   ret);
		fclose(f);
	}
	fclose(f);

	if ((ret = mbedtls_rsa_import(&rsa, &N, &P, &Q, &D, &E)) != 0)
	{
		mbedtls_printf(" failed\n  ! mbedtls_rsa_import returned %d\n\n",
					   ret);

	}

	if ((ret = mbedtls_rsa_complete(&rsa)) != 0)
	{
		mbedtls_printf(" failed\n  ! mbedtls_rsa_complete returned %d\n\n",
					   ret);
	}

	/*
     * Extract the RSA encrypted value from the text file
     */

	i = 0;

	while (i < 256)
	{
		buf[i] = input[i];
		i++;
	}

	if (i != rsa.len)
	{
		mbedtls_printf("\n  ! Invalid RSA signature format\n\n");
		// goto exit;
	}

	/*
     * Decrypt the encrypted RSA data and print the result.
     */

	fflush(stdout);

	ret = mbedtls_rsa_pkcs1_decrypt(&rsa, mbedtls_ctr_drbg_random,
									&ctr_drbg, MBEDTLS_RSA_PRIVATE, &i,
									buf, result, 1024);
	if (ret != 0)
	{
		mbedtls_printf(" failed\n  ! mbedtls_rsa_pkcs1_decrypt returned %d\n\n", ret);
	}
	
	int j = 0;

	while (j < 32)
	{
		output[j] = result[j];
		j++;
	}

	mbedtls_ctr_drbg_free(&ctr_drbg);
	mbedtls_entropy_free(&entropy);
	mbedtls_rsa_free(&rsa);
	mbedtls_mpi_free(&N);
	mbedtls_mpi_free(&P);
	mbedtls_mpi_free(&Q);
	mbedtls_mpi_free(&D);
	mbedtls_mpi_free(&E);
	mbedtls_mpi_free(&DP);
	mbedtls_mpi_free(&DQ);
	mbedtls_mpi_free(&QP);
}