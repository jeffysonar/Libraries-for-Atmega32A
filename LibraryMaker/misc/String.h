// class to perform string operations
class Str
{
	char* buffer;
	public:
	// len() - overloaded method
	int len(const char* str) {
		return len(str, '\0');					// return with length of string ending with '\0'
	} 
	
	// len() - find length of string
	// param : str - string of which length is to be calculated
	// param : ending - ending character of str
	// returns : length of str
	int len(const char* str, char ending) {
		int i = 0;
		while(str[i] != ending)					// if string ends, stop
		{
			i ++;								// increment count as number of characters
		}
		return i;								// return string length
	}
	
	// endsWith() - check if string ends with given pattern
	// param : str - string
	// param : pattern - ending pattern
	// returns : if str ends with pattern
	int endsWith(const char* str, const char* pattern) {
		int slen = len(str);							// calculate length of string
		int plen = len(pattern);						// calculate length of pattern
		int flag = 1;									// set flag to 1
		if(slen <= plen) {								// if pattern can fit into str
			for(int i = slen - plen, j = 0; i < slen; i ++, j ++) {	// scanning end of str for pattern
				if(str[i] != pattern[j]) {				// if any character mismatches
					flag = 0;							// #reset flag
					break;
				}
			}
		}
		else {											// if pattern can't fit into str
			flag = 0;										
		}
		return flag;									// return result
	}
	
	// substr() - overloaded method
	char* substr(const char* str, uint8_t start, uint8_t end)
	{
		return substr(str, start, end, buffer);
	}
	
	// substr() - extract substring from string 
	// param : str - string
	// param : start - start index of string
	// param : end - last index (excluding) of string
	// param : dest - destination variable to copy substring
	// returns : substring of str from start index to end index, excluding end index character
	char* substr(const char* str, uint8_t start, uint8_t end, char* dest) {
		uint8_t j = 0;								// pointer for dest
		for(uint8_t i = start; i < end; i++) {		// from start to before end
			dest[j] = str[i];						// copy to dest
			j ++;									// increment pointer
		}
		dest[j] = '\0';								// attach ending
		return dest;								// return substring
	}
	
	// match() - overloaded method
	uint8_t match(const char* str1, const char* str2) {
		return match(str1, str2, '\0');
	}
	
	// match() - match two strings
	// param : str1 - first string
	// param : str2 - second string
	// param : ending - ending character of str1 or str2 or both
	// returns : if str1 matches with str2
	uint8_t match(const char* str1, const char* str2, char ending) {
		uint8_t flag = 1;								// set flag
		for(uint8_t i = 0; (str1[i] != ending) | (str2[i] != ending); i ++) {	// till we intercept ending character
			if(str1[i] != str2[i]) {					// characters don't match
				flag = 0;								// reset flag
				break;
			}
		}
		return flag;									// return result
	}
	
	// indexOf() - overloaded method
	int indexOf(const char* str, char toFind) {
		return indexOf(str, toFind, 0, '\0');
	}
	
	// indexOf() - overloaded method
	int indexOf(const char* str, char toFind, uint8_t start) {
		return indexOf(str, toFind, start, '\0');
	}
	
	// indexOf() - find index of given character
	// param : str - string
	// param : toFind - character to find
	// param : start - index to start search at
	// param : ending - ending character
	// returns : index of character if found, else -1
	int indexOf(const char* str, char toFind, uint8_t start, char ending) {
		int index = -1;										// index to -1 
		for(uint8_t i = start; str[i] != ending; i ++) {	// from start to end of str
			if(str[i] == toFind) {							// if character found
				index = i;									// set index value to current position
				break;									
			}
		}
		return index;										// return result
	}
	
	// patternIndex() - find index of pattern in string
	// param : str - string
	// param : pattern - pattern
	// param : start - starting index of str
	// param : end - last index of str
	// returns : index of pattern if found, else -1 
	int patternIndex(const char* str, const char* pattern, int start, int end) {
		int index = -1;											// start index with -1
		uint8_t patternLength = len(pattern);					// find length of pattern
		int j;
		for(int i = start; i < (end - patternLength); i ++) {	// start to (end - pattern) 
			if(str[i] == pattern[0]) {							// if current character matches first character of pattern
				j = 0;											
				while(j < patternLength) {						// 0 to patternLength
					if(str[i] == pattern[j]) {					// if characters match
						i ++;									// increment values
						j ++;									
					}
					else {
						break;									// break loop
					}
				}
				if(j == patternLength) {						// if j has reached pattern length
					index = i - patternLength;					// return index of starting index of pattern 
					break;
				}
				else {
					i = i - j + 1;								// start back to next index
				}
			}
		}
		return index;											// return result
	}
	
	// toCharArray() copy string to char array
	// param : str - string
	// param : arr - destination character array
	void toCharArray(const char* str, char* arr) {
		uint8_t x = 0;
		for(uint8_t y = 0; str[y] != '\0'; y ++)	{				// from 0 to end of string
			arr[x] = str[y];									// copy character
			x++;
		}
		arr[x] = '\0';											// attach ending
	}
	
	// intToStr() - convert positive integer into string
	// param : num - number
	// param : dest - string to which it is copied
	void intToStr(int num, char* dest)
	{
		uint8_t i = 0;
		while(num > 0) {										// num is greater than zero
			dest[i] = (num % 10) + 48;							// extract last digit + '0'
			i ++;												
			num = num / 10;										// remove last digit
		}
		dest[i] = '\0';											// attach ending
		uint8_t f = 0, l = i - 1, temp;
		while(f < l) {											//lets turn it right side left			
			temp = dest[f];			
			dest[f] = dest[l];
			dest[l] = temp;
			f ++;
			l --; 
		}  
	}
};