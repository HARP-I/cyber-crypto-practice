#include <iostream>
#include <openssl/rand.h>
#include <openssl/evp.h>
#include <time.h>
using namespace std;
#include <unordered_map>
//输出结果
static void dump_buf(char* ciphertext_32, int lenth)
{
	for (int i = 0; i < lenth; i++) {
		printf("%02X ", (unsigned char)ciphertext_32[i]);
	}
	printf("\n");
}

int sm3_hash_openssl(uint8_t*dgst,const void *msg,size_t len) {
	int res = 0;
	const EVP_MD* md = EVP_get_digestbyname("sm3");
	EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
	if (!mdctx) goto done;

	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, msg, len);
	res = EVP_DigestFinal_ex(mdctx, dgst, NULL);

done:
	EVP_MD_CTX_free(mdctx);
	return res;
}

int sha256_hash_openssl(uint8_t* dgst, const void* msg, size_t len) {
	int res = 0;
	const EVP_MD* md = EVP_get_digestbyname("sha256");
	EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
	if (!mdctx) goto done;

	EVP_DigestInit_ex(mdctx, md, NULL);
	EVP_DigestUpdate(mdctx, msg, len);
	res = EVP_DigestFinal_ex(mdctx, dgst, NULL);

done:
	EVP_MD_CTX_free(mdctx);
	return res;
}
int sha256_hash_verify_openssl(const void* msg, size_t len, const void* dgst) {
	uint8_t buf[32];
	sha256_hash_openssl(buf, msg, len);
	return memcmp(msg,dgst,32);
}

int sm3_hash_verify_openssl(const void* msg, size_t len, const void* dgst) {
	uint8_t buf[32];
	sm3_hash_openssl(buf, msg, len);
	return memcmp(buf, dgst, 32);
}

string int_to_hex(uint8_t* list,int len) {
	string buff;
	char a[] = "0123456789abcdef";
	for (int i = 0; i < len; i++) {
		int b = list[i] >> 4;
		uint8_t c = uint8_t((list[i] <<4));
		c=c/16;
		buff.push_back(a[b]);
		buff.push_back(a[c]);
	}
	return buff;
}

char world1[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char world2[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char world3[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char world4[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char world5[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char world6[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
char world7[] = "abcdefghijklmnopqrstuvwxyz0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

int b_attack() {
	char msg[8];
	msg[7] = '\0';
	uint8_t dgst[32];
	unordered_map<string, string> m;
	msg[0] = world1[0];
	msg[1] = world2[0];
	msg[2] = world3[0];
	msg[3] = world4[0];
	msg[4] = world5[0];
	sm3_hash_openssl(dgst, msg, strlen(msg));
	//m.insert(pair<uint8_t[32], char[4]>(dgst,msg))
	string str1 = int_to_hex(dgst, 4);
	string str2 = msg;
	m[str1] = msg;
	//cout<< (m["aaa"]=="");
	for (int i = 0; i < 62; i++) {
		for (int j = 0; j <62; j++) {
			for (int k = 0; k < 62; k++) {
				for (int n = 0;  n< 62; n++) {
					for (int l = 1; l < 62; l++) {
						for (int p = 1; p < 62; p++) {
							for (int q = 1; q < 62; q++) {
								msg[0] = world1[i];
								msg[1] = world2[j];
								msg[2] = world3[k];
								msg[3] = world4[n];
								msg[4] = world5[l];
								msg[5] = world6[p];
								msg[6] = world7[q];
								sm3_hash_openssl(dgst, msg, strlen(msg));
								//dump_buf((char*)dgst, 32);
								//m.insert(pair<uint8_t[32], char[4]>(dgst,msg))
								string str1 = int_to_hex(dgst, 7);
								string str2 = msg;
								if (m[str1] != "") {
									dump_buf((char*)dgst, 32);
									cout << str2 << endl;
									msg[0] = m[str1][0]; msg[1] = m[str1][1]; msg[2] = m[str1][2]; msg[3] = m[str1][3]; msg[4] = m[str1][4];
									msg[5] = m[str1][5]; msg[6] = m[str1][6];
									sm3_hash_openssl(dgst, msg, strlen(msg));
									dump_buf((char*)dgst, 32);
									cout << m[str1] << endl;
									return 0;
								}
								m[str1] = msg;
							}
						}
					}
				}
			}
		}
	}
	

	//cout << "ok";
	//sm3_hash_verify_openssl(msg, strlen(msg), dgst);
	//sha256_hash_verify_openssl(msg, strlen(msg), dgst);
}

int main() {
	clock_t startTime = clock();
	b_attack();
	clock_t endTime = clock();
	cout << "整个程序用时：" << double(endTime - startTime) / CLOCKS_PER_SEC << "s" << endl;
	system("pause");
}