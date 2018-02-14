//Some definitions using NEON intrinsics.  This file
//is used for all of our NEON implementations of Simeck with 16-bit block sizes.

#include <arm_neon.h>
#include <stdint.h>

#define u16	uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define u128 uint32x4_t


#define XOR veorq_u32
#define AND vandq_u32
#define SL vshlq_n_u32
#define SR vshrq_n_u32

#define SET1(X,c) (X=vdupq_n_u32(c))

#define ST32(ip,X) vst1q_u32((u32 *)(ip),X)
#define LD32(ip) vld1q_u32((u32 *)(ip))


#define ROL1(X)  vsriq_n_u32(SL(X,1),X,31)
#define ROL5(X)  vsriq_n_u32(SL(X,5),X,27)


void Simeck_Enc(u32 *p, u32*k, u32 *c){
	u128 data1, data2, data3, data4;
	u128 data5, data6, data7, data8;
	u128 data9, data10, data11, data12;
	u128 data13, data14, data15, data16;
	
	int i = 0;
	//Plaintext Load
	data1 = LD32(p); // left
	data2 = LD32(p+4); // right
	data9 = LD32(p+8); //left2
	data10 = LD32(p+12); //right2

	//Encryption Round Function
	for(i=0; i<44; i++){
		data3 = ROL5(data1);
		data11 = ROL5(data9);
		
		data4 = AND(data1, data3);
		data12 = AND(data9, data11);

		data5 = ROL1(data1);
		data13 = ROL1(data9);

		data6 = XOR(data4, data2);
		data14 = XOR(data12, data10);

		SET1(data7, k[i]);

		data8 = XOR(data5, data6);
		data15 = XOR(data13, data14);

		data2 = data1;
		data10 = data9;

		data1 = XOR(data7, data8);
		data9 = XOR(data7, data15);		
	}

	//Ciphertext Save
	ST32(c, data1);
	ST32(c+4, data2);
	ST32(c+8, data9);
	ST32(c+12, data10);
}
