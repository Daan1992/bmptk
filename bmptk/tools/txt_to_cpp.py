import sys, os

def Raise( T ):
   print( T )
   exit( 1 )
        
def File_Extension( File_Name ):
   "return the file extension of File_Name (without the dot)"
   Dummy, Extension = os.path.splitext( File_Name )
   return Extension[ 1: ] if len( Extension ) > 0 else ""        
        
def File_Base( File_Name ):
   "return the base name of File_Name (without the dot+extension)"
   Base = os.path.basename( File_Name )
   return Base.replace( "." + File_Extension( Base ), "" )        
          
def Lines_From_Txt( Source, Name ):
   Result = ""
   for Line in Source.readlines():
      Result += '   "%s"\n' % Line
   return Result
   
def Txt_to_Cpp( Txt_File_Name, Cpp_File_Name ):
   Name = File_Base( Cpp_File_Name )
        
   try:   
      Txt = open( Txt_File_Name )
   except:
      Raise( "Error: source file %s could not be opened" % Img_File_Name )    
      
   Data = Lines_From_Txt( Txt, Name )

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
   
   Cpp.write( '#include "%s.h"\n' % Name )
   Cpp.write( 'const char %s[]=\n' % Name )
   Cpp.write( Data )
   Cpp.write( ';' )
   Cpp.close()
   
   Hpp.write( '#include "bmptk.h"\n' )
   Hpp.write( 'extern const char %s[];\n' % Name )
   Hpp.close()
      
def Run():
   pass      
      
if __name__ == '__main__':
   if len( sys.argv ) != 3:
      Raise( "usage: txt_to_cpp <txt-file-name> <cpp-and-h-file-name>" )
   Txt_to_Cpp( sys.argv[ 1 ], sys.argv[ 2 ] )
            
         