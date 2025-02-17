//Those are from the libImages.so lib.
extern unsigned char img_cat_cat01_bmp;
extern unsigned int  img_cat_cat01_bmp_len;

extern unsigned char img_cat_cat02_bmp;
extern unsigned int  img_cat_cat02_bmp_len;

extern unsigned char img_cat_cat03_bmp;
extern unsigned int  img_cat_cat03_bmp_len;

extern unsigned char img_cat_cat04_bmp;
extern unsigned int  img_cat_cat04_bmp_len;

extern unsigned char img_cat_cat05_bmp;
extern unsigned int  img_cat_cat05_bmp_len;

extern unsigned char img_cat_cat06_bmp;
extern unsigned int  img_cat_cat06_bmp_len;

extern unsigned char img_cat_cat07_bmp;
extern unsigned int  img_cat_cat07_bmp_len;

extern unsigned char img_cat_cat08_bmp;
extern unsigned int  img_cat_cat08_bmp_len;

extern unsigned char img_cat_cat09_bmp;
extern unsigned int  img_cat_cat09_bmp_len;

extern unsigned char img_cat_cat10_bmp;
extern unsigned int  img_cat_cat10_bmp_len;

extern unsigned char img_other_other1_bmp;
extern unsigned int img_other_other1_bmp_len;

extern unsigned char img_dog_dog1_bmp;
extern unsigned int img_dog_dog1_bmp_len;

extern unsigned char img_dog_dog2_bmp;
extern unsigned int img_dog_dog2_bmp_len;

extern unsigned char img_dog_dog3_bmp;
extern unsigned int img_dog_dog3_bmp_len;

extern unsigned char img_dog_dog4_bmp;
extern unsigned int img_dog_dog4_bmp_len;

extern unsigned char img_dog_dog5_bmp;
extern unsigned int img_dog_dog5_bmp_len;


/** An array containing pointers to all image dumps */
inline const unsigned char* allDump[] = {
	&img_cat_cat01_bmp,
	&img_cat_cat02_bmp,
	&img_cat_cat03_bmp,
	&img_cat_cat04_bmp,
	&img_cat_cat05_bmp,
	&img_cat_cat06_bmp,
	&img_cat_cat07_bmp,
	&img_cat_cat08_bmp,
	&img_cat_cat09_bmp,
	&img_cat_cat10_bmp,
	&img_dog_dog1_bmp,
	&img_dog_dog2_bmp,
	&img_dog_dog3_bmp,
	&img_dog_dog4_bmp,
	&img_dog_dog5_bmp,
};
/** The size of `allDump` */
inline constexpr size_t allDumpSize = 17;

/** An array containing the length of all image dumps (sorted in the same order as `allDump`) */
inline const unsigned int allDumpLen[] = {
	img_cat_cat01_bmp_len,
	img_cat_cat02_bmp_len,
	img_cat_cat03_bmp_len,
	img_cat_cat04_bmp_len,
	img_cat_cat05_bmp_len,
	img_cat_cat06_bmp_len,
	img_cat_cat07_bmp_len,
	img_cat_cat08_bmp_len,
	img_cat_cat09_bmp_len,
	img_cat_cat10_bmp_len,
	img_other_other1_bmp_len,
	img_dog_dog1_bmp_len,
	img_dog_dog2_bmp_len,
	img_dog_dog3_bmp_len,
	img_dog_dog4_bmp_len,
	img_dog_dog5_bmp_len,
};
/** The size of `allDumpLen` */
inline constexpr size_t allDumpLenSize = 17;

/** How many cat sprites there are in `allDump`. */
inline constexpr uint8_t catCount	= 10;

/** How many animal sprites there are in `allDump`. */
inline constexpr uint8_t otherCount	= 1;

/** How many dog sprites there are in `allDump`. */
inline constexpr uint8_t dogCount	= 5;