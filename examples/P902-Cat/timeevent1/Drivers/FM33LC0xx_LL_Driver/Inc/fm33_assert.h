#ifndef FM33_ASSERT_H
#define FM33_ASSERT_H

#ifndef DEBUG
 #define assert_param(b)  
#else
 #define assert_param(b) if(!(b)) while(1){}
#endif	
	



#endif 

