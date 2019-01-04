#include <stdio.h>
#include "yxyDES.h"

void main()
{
	yxyDES* my_des = new yxyDES();
	string test_string = "test des plaintext!";
	printf("first,we use the des:\n");                  //先用DES
	//initialize key
	my_des->InitializeKey("12345678",false);            //加密密码
	printf("key is : 12345678 \n");
	//use des to encrypt
	my_des->EncryptAnyLength(test_string,false);		//加密
	printf("set plaintext : %s \n",test_string.c_str());//加密原文件为test_string.cstr()
	//get the ciphertext
	test_string = my_des->GetCiphertextAnyLength();		//获取密文
	printf("after encrypt : %s \n",test_string.c_str());
	//use des to decrypt
	printf("now,decrypting...\n");
	my_des->DecryptAnyLength(test_string,false);		//解密
	test_string = my_des->GetPlaintextAnyLength();		//解密后文件为test_string.cstr()
	printf("after decrypt : %s \n",test_string.c_str());
	

	//use 3des							
	printf("\n\nnow,let us use the 3des:\n");			//再用3DES
	printf("before encrypt,test_string is : %s\n",test_string.c_str());
	//initialize the first key	
	my_des->InitializeKey("12345678",false);			//加密密码
	printf("the first key is : 12345678 \n");
	//initialize the second key
	my_des->InitializeKey("87654321",true);				//第二个密码
	printf("the second key is : 87654321 \n");
	//use 3des to encrypt
	my_des->TripleEncryptAnyLength(test_string);		//加密
	//get the ciphertext
	test_string = my_des->GetCiphertextAnyLength();		//获取密文
	printf("after encrypt : %s \n",test_string.c_str());
	//use the 3des to decrypt
	printf("now,(3des)decrypting...\n");
	my_des->TripleDecryptAnyLength(test_string);		//解密
	test_string = my_des->GetPlaintextAnyLength();		//解密后文件为test_string.cstr()
	printf("after (3des)decrypt : %s \n",test_string.c_str());
	getchar();
	system("pause");
}