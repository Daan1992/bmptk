import sys, os

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
        
# 1 bit per pixel        
def Lines_From_Image_Old( Image, Name, Invert = 0 ):
   Result = ""
   Result += "const unsigned char %s_data[] = { \n" % Name
   x_size, y_size = Image.size
   comma = " "
   for y in range( y_size ):
      m = 0x80
      d = 0
      S = "/* y = %03d */ " % y
      for x in range( x_size ):
         pixel = Image.getpixel((x,y))
         try:
            red, green, blue = pixel
            v = (( 255 - red ) + ( 255 - green ) + ( 255 - blue ))
         except:
            v = ( 255 - pixel )            
         if Invert: 
            if ( v > 0 ):
               d = d + m         
         else:
            if ( v <= 0 ):
               d = d + m
         if 0: print( x, y, pixel, v, m, d )
         m = m / 2
         if( m == 0 ) | ( x == x_size-1):
            # print( "====> %02X" % d )
            S += ( "%s0x%02X" % ( comma, d ))
            m = 0x80
            d = 0
            comma = ","
         if len( S ) > 70:
            Result += "   %s\n" % S 
            S = ""
      Result += "   /* */ %s\n" % S  # ???
   Result += "   %s\n" % S
   Result += "};\n" 
   return ( x_size, y_size, Result )
   
def Lines_From_Image( Image, Name, Invert = 0 ):
   print( Image.getbands())
   Result = ""
   Result += "const unsigned char %s_data[] = { \n" % Name
   x_size, y_size = Image.size
   comma = " "
#   P = Image.palette
#   print( len( P.palette),  P.palette )
   for y in range( y_size ):
      S = "/* y = %03d */ " % y
      for x in range( x_size ):
         pixel = Image.getpixel((x,y))
#         red = ord( P.palette[ pixel ] )
#         green = ord( P.palette[ 256 + pixel ] )
#         blue = ord( P.palette[ 512 + pixel ] )
         red, green, blue = pixel
         S += "%s 0x%02X, 0x%02X, 0x%02X" % ( comma, red, green, blue )
         comma = ","
         if len( S ) > 70:
            Result += "   %s\n" % S 
            S = ""
   Result += "   %s\n" % S
   Result += "};\n" 
   return ( x_size, y_size, Result )
   
def Cpp_To_Img( Img_File_Name, Cpp_File_Name ):
   Name = File_Base( Cpp_File_Name )
   
   try:
      from PIL import Image
   except:
      Raise( "Error: the python PIL module is not installed." )  
           
   try:   
      Image = Image.open( Img_File_Name )
   except:
      Raise( "Error: source file %s could not be opened" % Img_File_Name )    
   X, Y, Data = Lines_From_Image( Image, Name )

   Cpp_File_Name = Name + ".cpp"
   try:
      Cpp = open( Cpp_File_Name, "w" )   
   except:
      Raise( "Error: destination file %s could not be opened" % Cpp_File_Name ) 
      
   Hpp_File_Name = Name + ".h"
   try:
      Hpp = open( Hpp_File_Name, "w" )   
   except:
      Raise( "Error: destination file %s could not be opened" % Hpp_File_Name )     
   
   Cpp.write( '#include "bmptk.h"\n' )
   Cpp.write( '#include "%s.h"\n' % Name )
   Cpp.write( 'using namespace bmptk;\n' )
   Cpp.write( 'using namespace graphics;\n' )
   Cpp.write( Data )
   Cpp.write( 'inline_rgb_photo %s(\n' % Name )   
   Cpp.write( '   vector( %d, %d ),\n' % ( X, Y ))   
   Cpp.write( '   %s_data\n' % Name)   
   Cpp.write( ');\n' )   
   Cpp.close()
   
   Hpp.write( '#include "bmptk.h"\n' )
   Hpp.write( 'extern bmptk::graphics::inline_rgb_photo %s;\n' % Name )
   Hpp.close()
      
def Run():
   pass      
      
if __name__ == '__main__':
   if len( sys.argv ) != 3:
      Raise( "usage: img_to_cpp <img-file-name> <cpp-and-h-file-name>" )
   Cpp_To_Img( sys.argv[ 1 ], sys.argv[ 2 ] )
            
         