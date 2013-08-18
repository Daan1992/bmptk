void *memcpy( unsigned char *s1, unsigned char *s2, unsigned int n){
   while( n-- ){
      *s1++ = *s2++;
   }   
}