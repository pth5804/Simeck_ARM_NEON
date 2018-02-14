//Some definitions using NEON intrinsics.  This file
//is used for all of our NEON implementations of Simeck with 16-bit block sizes.

#include <arm_neon.h>
#include <stdint.h>

#define u16	uint16_t
#define u32 uint32_t
#define u64 uint64_t
#define u128 uint16x8_t

#define LCS(x,r) (((x)<<r)|((x)>>(32-r)))
#define RCS(x,r) (((x)>>r)|((x)<<(32-r)))

#define XOR veorq_u16
#define AND vandq_u16
#define ADD vaddq_u16
#define SL vshlq_n_u16
#define SR vshrq_n_u16

#define SET1(X,c) (X=vdupq_n_u16(c))

#define ST16(ip,X) vst1q_u16((u16 *)(ip),X)
#define LD16(ip) vld1q_u16((u16 *)(ip))

#define ROL1(X)  vsriq_n_u16(SL(X,1),X,15)
#define ROL5(X)  vsriq_n_u16(SL(X,5),X,11)


void Simeck_Enc(u16 *p, u16*k, u16 *c){
	u128 data1, data2, data3, data4;
	u128 data5, data6, data7, data8;
	u128 data9, data10, data11, data12;
	u128 data13, data14, data15, data16;
	
	int i = 0;
	//Plaintext Load
	data1 = LD16(p); // left
	data2 = LD16(p+8); // right
	data9 = LD16(p+16); //left2
	data10 = LD16(p+24); //right2

	//Encryption Round Function
	for(i=0; i<32; i++){
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
	ST16(c, data1);
	ST16(c+8, data2);
	ST16(c+16, data9);
	ST16(c+24, data10);
}
