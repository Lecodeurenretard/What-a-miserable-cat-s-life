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
	&img_other_other1_bmp
};
/** The size of `allDump` */
inline constexpr size_t allDumpSize = 11;

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
	img_other_other1_bmp_len
};
/** The size of `allDumpLen` */
inline constexpr size_t allDumpLenSize = 11;