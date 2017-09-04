# Example Config File for loading Wrapper DLLs  
**Author: Sean Pesce**  

This is just an example of how wrapper DLLs could be set for loading using the config file. If a wrapper DLL is specified in the config file, ReShade should load the wrapper in place of the real DLL, and it is assumed that each wrapper DLL will subsequently load the corresponding real DLL (or another wrapper DLL to continue the chain, etc).  

My fork implements basic wrapper DLL loading using this config file layout, simply as a proof of concept; it should definitely be re-implemented in a more robust fashion.  
