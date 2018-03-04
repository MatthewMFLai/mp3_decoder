#ifndef UTIL_H__
#define UTIL_H__

#define CONFIG_BYTE_DIGITS_MAX		3
#define CONFIG_WORD_DIGITS_MAX		5 
#define CONFIG_LONGWORD_DIGITS_MAX	10
#define BCD_MULTIPLIER				16
#define DIVISOR_BYTE				100
#define DIVISOR_WORD				10000
#define DIVISOR_LONGWORD 			1000000000
#define ZERO						'0'
#define MINUS						'-'
#define DIVISOR						10
#define SCALER						10
#define TWO_DIGIT_VALUE             10
									
uint8_t byte_to_ascii (uint8_t *p_dest, uint8_t value);
uint8_t word_to_ascii (uint8_t *p_dest, uint16_t value);
uint8_t longword_to_ascii (uint8_t *p_dest, uint32_t value);
uint8_t ascii_to_byte (uint8_t *p_data, uint8_t len);
uint16_t ascii_to_word (uint8_t *p_data, uint8_t len);
uint32_t ascii_to_longword (uint8_t *p_data, uint8_t len);
uint8_t ascii_to_bcd (char msn, char lsn);
void big_to_small_endian(uint8_t *p_data, uint8_t len);
uint8_t signed_byte_to_ascii (uint8_t *p_dest, int8_t signed_value);
void byte_to_hex(char *p_msnibble, char *p_lsnibble, uint8_t value);
void string_to_bcd(char *p_src, uint16_t len, char *p_dest);

uint8_t calc_checksum(uint8_t *s);

#endif  /* _ UTIL_H__ */
