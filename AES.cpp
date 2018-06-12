#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#define Nb 4
#define Nr 10
#define Nk 4
#define xtime(x)   ((x<<1) ^ (((x>>7) & 1) * 0x1b))
#define Multiply(x,y) (((y & 1) * x) ^ ((y>>1 & 1) * xtime(x)) ^ ((y>>2 & 1) * xtime(xtime(x))) ^ ((y>>3 & 1) * xtime(xtime(xtime(x)))) ^ ((y>>4 & 1) * xtime(xtime(xtime(xtime(x))))))
using namespace std;
class AES {
    private:
        unsigned char in[16], out[16], state[4][4];
        unsigned char RoundKey[240];
        unsigned char Key[16];
        int sbox[256] =   {	0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76,
                            0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0,
                            0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15,
                            0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75,
                            0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84,
                            0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf,
                            0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8,
                            0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2,
                            0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73,
                            0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb,
                            0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79,
                            0xe7, 0xc8, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08,
                            0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a,
                            0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e,
                            0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe9, 0xce, 0x55, 0x28, 0xdf,
                            0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 };
        int rsbox[256] =  { 0x52, 0x09, 0x6a, 0xd5, 0x30, 0x36, 0xa5, 0x38, 0xbf, 0x40, 0xa3, 0x9e, 0x81, 0xf3, 0xd7, 0xfb,
                            0x7c, 0xe3, 0x39, 0x82, 0x9b, 0x2f, 0xff, 0x87, 0x34, 0x8e, 0x43, 0x44, 0xc4, 0xde, 0xe9, 0xcb,
                            0x54, 0x7b, 0x94, 0x32, 0xa6, 0xc2, 0x23, 0x3d, 0xee, 0x4c, 0x95, 0x0b, 0x42, 0xfa, 0xc3, 0x4e,
                            0x08, 0x2e, 0xa1, 0x66, 0x28, 0xd9, 0x24, 0xb2, 0x76, 0x5b, 0xa2, 0x49, 0x6d, 0x8b, 0xd1, 0x25,
                            0x72, 0xf8, 0xf6, 0x64, 0x86, 0x68, 0x98, 0x16, 0xd4, 0xa4, 0x5c, 0xcc, 0x5d, 0x65, 0xb6, 0x92,
                            0x6c, 0x70, 0x48, 0x50, 0xfd, 0xed, 0xb9, 0xda, 0x5e, 0x15, 0x46, 0x57, 0xa7, 0x8d, 0x9d, 0x84,
                            0x90, 0xd8, 0xab, 0x00, 0x8c, 0xbc, 0xd3, 0x0a, 0xf7, 0xe4, 0x58, 0x05, 0xb8, 0xb3, 0x45, 0x06,
                            0xd0, 0x2c, 0x1e, 0x8f, 0xca, 0x3f, 0x0f, 0x02, 0xc1, 0xaf, 0xbd, 0x03, 0x01, 0x13, 0x8a, 0x6b,
                            0x3a, 0x91, 0x11, 0x41, 0x4f, 0x67, 0xdc, 0xea, 0x97, 0xf2, 0xcf, 0xce, 0xf0, 0xb4, 0xe6, 0x73,
                            0x96, 0xac, 0x74, 0x22, 0xe7, 0xad, 0x35, 0x85, 0xe2, 0xf9, 0x37, 0xe8, 0x1c, 0x75, 0xdf, 0x6e,
                            0x47, 0xf1, 0x1a, 0x71, 0x1d, 0x29, 0xc5, 0x89, 0x6f, 0xb7, 0x62, 0x0e, 0xaa, 0x18, 0xbe, 0x1b,
                            0xfc, 0x56, 0x3e, 0x4b, 0xc6, 0xd2, 0x79, 0x20, 0x9a, 0xdb, 0xc0, 0xfe, 0x78, 0xcd, 0x5a, 0xf4,
                            0x1f, 0xdd, 0xa8, 0x33, 0x88, 0x07, 0xc7, 0x31, 0xb1, 0x12, 0x10, 0x59, 0x27, 0x80, 0xec, 0x5f,
                            0x60, 0x51, 0x7f, 0xa9, 0x19, 0xb5, 0x4a, 0x0d, 0x2d, 0xe5, 0x7a, 0x9f, 0x93, 0xc9, 0x9c, 0xef,
                            0xa0, 0xe0, 0x3b, 0x4d, 0xae, 0x2a, 0xf5, 0xb0, 0xc8, 0xeb, 0xbb, 0x3c, 0x83, 0x53, 0x99, 0x61,
                            0x17, 0x2b, 0x04, 0x7e, 0xba, 0x77, 0xd6, 0x26, 0xe1, 0x69, 0x14, 0x63, 0x55, 0x21, 0x0c, 0x7d };
        int Rcon[255] = {	0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a,
                            0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39,
                            0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a,
                            0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8,
                            0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef,
                            0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc,
                            0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80, 0x1b,
                            0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3,
                            0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94,
                            0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04, 0x08, 0x10, 0x20,
                            0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63, 0xc6, 0x97, 0x35,
                            0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd, 0x61, 0xc2, 0x9f,
                            0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb, 0x8d, 0x01, 0x02, 0x04,
                            0x08, 0x10, 0x20, 0x40, 0x80, 0x1b, 0x36, 0x6c, 0xd8, 0xab, 0x4d, 0x9a, 0x2f, 0x5e, 0xbc, 0x63,
                            0xc6, 0x97, 0x35, 0x6a, 0xd4, 0xb3, 0x7d, 0xfa, 0xef, 0xc5, 0x91, 0x39, 0x72, 0xe4, 0xd3, 0xbd,
                            0x61, 0xc2, 0x9f, 0x25, 0x4a, 0x94, 0x33, 0x66, 0xcc, 0x83, 0x1d, 0x3a, 0x74, 0xe8, 0xcb  };
        /*
	Далее находятся методы, используемые для шифрования/разшифровки текста пользователя 
	*/
        void KeyExpansion();
        void AddRoundKey(int round);
        void SubBytes();
        void InvSubBytes();
        void ShiftRows();
        void InvShiftRows();
        void MixColumns();
        void InvMixColumns();

    public:
        AES(unsigned char* k);
        void Cipher();
        void InvCipher();
        void SetInput(unsigned char* input);
        unsigned char *GetOutput();
        void ChangeKey(unsigned char* newKey);
};
AES::AES(unsigned char* k) {
    for(int i=0;i<Nk*4;i++)
	{
		Key[i]=k[i];
	}
	KeyExpansion();
}

void AES::KeyExpansion() {
	int i,j;
	unsigned char temp[4],k;

	// Первый раундовый ключ это сам ключ
	for(i=0;i<Nk;i++) {
		RoundKey[i*4]=Key[i*4];
		RoundKey[i*4+1]=Key[i*4+1];
		RoundKey[i*4+2]=Key[i*4+2];
		RoundKey[i*4+3]=Key[i*4+3];
	}

	// Все другие раундовые ключи формируются из предъидущих раундовых ключей
	while (i < (Nb * (Nr+1))) {
        for(j=0;j<4;j++) {
            temp[j]=RoundKey[(i-1) * 4 + j];
        }

        if (i % Nk == 0) {
	    // Эта функция изменяет порядок 4 байт 
			// [a0,a1,a2,a3] становится [a1,a2,a3,a0]
			// Функция RotWord()
			k = temp[0];
			temp[0] = temp[1];
			temp[1] = temp[2];
			temp[2] = temp[3];
			temp[3] = k;

			//SubWord () - это функция, которая принимает четырехбайтовое входное слово и
                        // применяет S-box к каждому из четырех байтов для создания выходного слова
			// Функция Subword()
			temp[0]=sbox[temp[0]];
			temp[1]=sbox[temp[1]];
			temp[2]=sbox[temp[2]];
			temp[3]=sbox[temp[3]];

			temp[0] =  temp[0] ^ Rcon[i/Nk];
		}
		else if (Nk > 6 && i % Nk == 4) {
            // Функция Subword()
            temp[0]=sbox[temp[0]];
			temp[1]=sbox[temp[1]];
			temp[2]=sbox[temp[2]];
			temp[3]=sbox[temp[3]];
        }
		RoundKey[i*4+0] = RoundKey[(i-Nk)*4+0] ^ temp[0];
		RoundKey[i*4+1] = RoundKey[(i-Nk)*4+1] ^ temp[1];
		RoundKey[i*4+2] = RoundKey[(i-Nk)*4+2] ^ temp[2];
		RoundKey[i*4+3] = RoundKey[(i-Nk)*4+3] ^ temp[3];
		i++;
	}
};

void AES::AddRoundKey(int round){
    int i,j;
	for(i=0;i<4;i++) {
		for(j=0;j<4;j++) {
			state[j][i] ^= RoundKey[round * Nb * 4 + i * Nb + j];
		}
	}
};

void AES::SubBytes(){
    int i,j;
	for(i=0;i<4;i++) {
		for(j=0;j<4;j++) {
			state[i][j] = sbox[(state[i][j])];
		}
	}
};

void AES::InvSubBytes(){
    int i,j;
	for(i=0;i<4;i++) {
		for(j=0;j<4;j++) {
			state[i][j] = rsbox[(state[i][j])];
		}
	}
};

void AES::ShiftRows(){
	unsigned char temp;

	// Поворачивание первой строки 1-го столбца влево
	temp=state[1][0];
	state[1][0]=state[1][1];
	state[1][1]=state[1][2];
	state[1][2]=state[1][3];
	state[1][3]=temp;

	// Поворачивание второй строки 2-го столбца влево
	temp=state[2][0];
	state[2][0]=state[2][2];
	state[2][2]=temp;

	temp=state[2][1];
	state[2][1]=state[2][3];
	state[2][3]=temp;

	// Поворачивание третьей строки 3-го столбца влево
	temp=state[3][0];
	state[3][0]=state[3][3];
	state[3][3]=state[3][2];
	state[3][2]=state[3][1];
	state[3][1]=temp;
};

void AES::InvShiftRows(){
	unsigned char temp;

	// Поворачивание первой строки 1-го столбца вправо
	temp=state[1][3];
	state[1][3]=state[1][2];
	state[1][2]=state[1][1];
	state[1][1]=state[1][0];
	state[1][0]=temp;

	// Поворачивание второй строки 2-го столбца вправо
	temp=state[2][0];
	state[2][0]=state[2][2];
	state[2][2]=temp;

	temp=state[2][1];
	state[2][1]=state[2][3];
	state[2][3]=temp;

	// Поворачивание третьей строки 3-го столбца вправо
	temp=state[3][0];
	state[3][0]=state[3][1];
	state[3][1]=state[3][2];
	state[3][2]=state[3][3];
	state[3][3]=temp;
};

void AES::MixColumns(){
	int i;
	unsigned char Tmp,Tm,t;
	for(i=0;i<4;i++) {
		t=state[0][i];
		Tmp = state[0][i] ^ state[1][i] ^ state[2][i] ^ state[3][i] ;
		Tm = state[0][i] ^ state[1][i] ; Tm = xtime(Tm); state[0][i] ^= Tm ^ Tmp ;
		Tm = state[1][i] ^ state[2][i] ; Tm = xtime(Tm); state[1][i] ^= Tm ^ Tmp ;
		Tm = state[2][i] ^ state[3][i] ; Tm = xtime(Tm); state[2][i] ^= Tm ^ Tmp ;
		Tm = state[3][i] ^ t ; Tm = xtime(Tm); state[3][i] ^= Tm ^ Tmp ;
	}
};

void AES::InvMixColumns(){
    int i;
	unsigned char a,b,c,d;
	for(i=0;i<4;i++) {

		a = state[0][i];
		b = state[1][i];
		c = state[2][i];
		d = state[3][i];


		state[0][i] = Multiply(a, 0x0e) ^ Multiply(b, 0x0b) ^ Multiply(c, 0x0d) ^ Multiply(d, 0x09);
		state[1][i] = Multiply(a, 0x09) ^ Multiply(b, 0x0e) ^ Multiply(c, 0x0b) ^ Multiply(d, 0x0d);
		state[2][i] = Multiply(a, 0x0d) ^ Multiply(b, 0x09) ^ Multiply(c, 0x0e) ^ Multiply(d, 0x0b);
		state[3][i] = Multiply(a, 0x0b) ^ Multiply(b, 0x0d) ^ Multiply(c, 0x09) ^ Multiply(d, 0x0e);
	}
};

void AES::Cipher(){
	int i,j,round=0;

	// Копирование введённого PlainText в массив состояний
	for(i=0;i<4;i++) {
		for(j=0;j<4;j++) {
			state[j][i] = in[i*4 + j];
		}
	}

	// Добавление ключа первого раунда в состояние перед началом раундов
	AddRoundKey(0);

	// Тут будет Nr раундов.
	// Первые Nr-1 раундов идентичные
	// Эти Nr-1 раунды выполняются в цикле ниже
	for(round=1;round<Nr;round++) {
		SubBytes();
		ShiftRows();
		MixColumns();
		AddRoundKey(round);
	}

	// Последный раунд приведён ниже
	// Функция MixColumns нет в последнем раунде
	SubBytes();
	ShiftRows();
	AddRoundKey(Nr);

	// Процесс шифрования завершён
	// Копируется массив состояний в выходной массив
	for(i=0;i<4;i++) {
		for(j=0;j<4;j++) {
			out[i*4+j]=state[j][i];
		}
	}
};

void AES::InvCipher(){
    int i,j,round=0;

	//Копируем входной CipherText в массив состояний 
	for(i=0;i<4;i++) {
		for(j=0;j<4;j++) {
			state[j][i] = in[i*4 + j];
		}
	}

	// Добавление ключа первого раунда в состояние перед началом раундов
	AddRoundKey(Nr);

	// Будет Nr раундов
	// Первые Nr-1 раундов идентичны
	// Эти Nr-1 раунды выполняются в цикле ниже
	for(round=Nr-1;round>0;round--) {
		InvShiftRows();
		InvSubBytes();
		AddRoundKey(round);
		InvMixColumns();
	}

	// Последный раунд приведён ниже
	// Функция MixColumns нет в последнем раунде
	InvShiftRows();
	InvSubBytes();
	AddRoundKey(0);

	// Процесс разшифровки закончен
	// Копируется массив состояний в выходной массив
	for(i=0;i<4;i++) {
		for(j=0;j<4;j++) {
			out[i*4+j]=state[j][i];
		}
	}
};

void AES::SetInput(unsigned char* input) {
    for(int i=0;i<Nk*4;i++)
	{
		in[i]=input[i];
	}
};

unsigned char *AES::GetOutput() {
    return out;
};

void AES::ChangeKey(unsigned char* newKey) {
    for(int i=0;i<Nk*4;i++)
	{
		Key[i]=newKey[i];
	}
	KeyExpansion();
};
int main() {

    cout << " --- Тест AES --- \n";

   unsigned char k[16];
   printf("Введите ключ шифрования: ");
   cin >> k;
    unsigned char *output = new unsigned char[16];
    AES *aes;

    aes = new AES(k);

 unsigned char input[16];
cout << "Введите сообщение для шифрования ";
  cin >> input;
     cout <<"\nТекст до шифрования:\n";
	for(int i=0;i<Nk*4;i++)	{
		printf("%02x ",input[i]);
	}
	 cout << "\nASCII - " << input << "\n\n"; // Вывод в ASCII-символах

    aes->SetInput(input);
    aes->Cipher();
    output = aes->GetOutput();

    cout << "\nТекст после шифрования:\n";
	for(int i=0;i<Nk*4;i++) {
		printf("%02x ",output[i]);
	}
	 cout << "\nASCII - " << output << "\n\n"; // Вывод в ASCII-символах

	for(int i=0;i<Nk*4;i++) {
		input[i] = output[i];
	}
	aes->SetInput(input);
    aes->InvCipher();
    output = aes->GetOutput();

    cout << "\nТекст после расшифровки:\n";
	for(int i=0;i<Nk*4;i++)	{
		printf("%02x ",output[i]);
	}
	cout << "\nASCII - " << output << "\n\n"; // Вывод в ASCII-символах
    return 0;
}
