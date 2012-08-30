import sys, os

try:
   import PIL
   import ImageFont
   from PIL import ImageFont
except:
      Errors.Raise( "Error: the python PIL module is not installed." )  

def Raise( T ):
   print( T )
   exit( 1 )

def Pixel_To_Bit(pixel,threshold=128):
    """Convert the pixel value to a bit."""
    if pixel > threshold:
        return 1
    else:
        return 0
        
def File_Extension( File_Name ):
   "return the file extension of File_Name (without the dot)"
   Dummy, Extension = os.path.splitext( File_Name )
   return Extension[ 1: ] if len( Extension ) > 0 else ""        
        
def File_Base( File_Name ):
   "return the base name of File_Name (without the dot+extension)"
   Base = os.path.basename( File_Name )
   return Base.replace( "." + File_Extension( Base ), "" )        
   
def Lines_From_Image( Image, Object_Name, Invert = 0 ):
   Result = ""
   Result += "const unsigned char %s_data[] = { \n" % Object_Name
   x_size, y_size = Image.size
   comma = " "
   m = 0x01
   d = 0
   S = ""
   for y in range( y_size ):
      for x in range( x_size ):
         pixel = Image.getpixel((x,y))
         v = ( 255 - pixel )            
         if Invert: 
            if ( v > 0 ):
               d = d + m         
         else:
            if ( v <= 0 ):
               d = d + m
         if 0: print( x, y, pixel, v, m, d )
         m = m * 2
         if( m == 0x100 ):
            S += ( "%s0x%02X" % ( comma, d ))
            m = 0x01
            d = 0
            comma = ","
            if len( S ) > 70:
               Result += "   %s\n" % S 
               S = ""
   Result += "   %s\n" % S
   Result += "};\n"
   return Result  
          
def Lines_From_Font( Font, Name, List ): 
      
   Result = ""   
   Chars = ""   
   List = "0"
   Max = 0
   Min = 10000
   Last = 0
   Mode = "1"
   
   for n in range( 0, 128 ): 
      c = chr( n )
      Chars += c    
      ( Total, Height ) = Font.getmask( Chars, mode = Mode ).size
      Len = Total - Last
      if Len < Min : Min = Len
      if Len > Max : Max = Len
      List = List + ( ",%d" % Total)
      Last = Total
      
   Image = Font.getmask( Chars, mode = Mode )
   
   if 0: print( Min, Max, List )
   #if Min != Max:
   #   Errors.Raise( "font %s is not fixed-size" % File_Name )
   #Image.show()
   #Image.save( Name + '.jpg', "JPEG" )
   
   Result = Lines_From_Image( Image, Name )
   
   Width = 0
   Max = -1 # force
   if Max == Min:
      Width = Max
      Start_Address = "0"
   else:
      Result += "const int %s_start[] = {\n" % Name
      Result += "   %s\n" % List;
      Result +=  "};\n"
      Start_Address = Name + "_Start"

   return 0, ( Width, Height ), Image.size , Result
   
def Ttf_To_Img( Img_File_Name, Code_File_Name, Name, Size, List = None, Namespace = None ):
   if List == None:   
      List = ""
      for x in range ( 0x00, 0x7F ):
         List += chr( x )
         
   try:   
      Font = ImageFont.truetype( Img_File_Name, int( Size ))
   except:
      Raise( "Error: source file %s could not be opened" % Img_File_Name )    
      
   Fixed, ( Char_X, Char_Y ), ( Pic_X, Pic_Y ), Data = \
      Lines_From_Font( Font, Name, List )

   Cpp_File_Name = Code_File_Name + ".cpp"
   try:
      Cpp = open( Cpp_File_Name, "w" )   
   except:
      Raise( "Error: destination file %s could not be opened" % Cpp_File_Name ) 
      
   Hpp_File_Name = Code_File_Name + ".h"
   try:
      Hpp = open( Hpp_File_Name, "w" )   
   except:
      Raise( "Error: destination file %s could not be opened" % Hpp_File_Name )     
   
   Cpp.write( '// source: %s \n' % Img_File_Name )
   Cpp.write( '// size: %d \n' % Size )
   Cpp.write( '#include "bmptk.h"\n' )
   Cpp.write( '#include "%s"\n' % Hpp_File_Name )

   Cpp.write( 'using namespace bmptk; \n' )
   if Namespace != None:
      Cpp.write( 'namespace %s {\n' % Namespace )
   Cpp.write( 'extern const int %s_start[];\n' % Name )
   Cpp.write( 'extern const unsigned char %s_data[];\n' % Name )
   Cpp.write( 'const inline_font & %s(){ \n' % Name )
   Cpp.write( '   static inline_font fnt(\n' )
   Cpp.write( '      %d,\n' % Fixed )
   Cpp.write( '      vector( %d, %d ),\n' % ( Char_X, Char_Y ))
   Cpp.write( '      %s_start,\n' % Name )
   Cpp.write( '      vector( %d, %d ), %s_data\n' % ( Pic_X, Pic_Y, Name ))
   Cpp.write( '   );\n' )   
   Cpp.write( '   return fnt;\n' )
   Cpp.write( '}\n' )
   Cpp.write( Data )  
   if Namespace != None:
      Cpp.write( '}\n' ) 
   Cpp.close()
   
   Hpp.write( '#include "bmptk.h"\n' )
   if Namespace != None:
      Hpp.write( 'namespace %s {\n' % Namespace )
   Hpp.write( 'const bmptk::inline_font & %s();\n' % Name )
   if Namespace != None:
      Hpp.write( '}\n' ) 
   Hpp.close()
      
def Run():
   pass      
      
if __name__ == '__main__':
   if not ( len( sys.argv ) in [ 6, 7 ] ):
      # print( len(sys.argv ))
      Raise( 'usage: \n' + 
         'fft_to_cpp <fft-file> <cpp-or-h-file> <name> <font-size> "<characters>" [<namespace>]' )
      
   try:   
      Size = int( sys.argv[ 4 ] )  
   except:   
      Raise( '<font-size> argument [%s] is not a valid integer' % sys.argv[ 3 ] )
      
   List = sys.argv[ 5 ]
   if List == "":
      List = None   
      
   try:
      Namespace = sys.argv[ 6 ]
   except:
      Namespace = None    
      
   Ttf_To_Img( sys.argv[ 1 ], sys.argv[ 2 ], sys.argv[ 3 ], Size, List, Namespace )
            
         
