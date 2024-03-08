In the preCompiler deal with a macro beeing called from inside a label
"Loop: macro_name"

Check the name of a Label: can't be a saved name (register,,)
Maybe also check the name of a macro

In the compile-first-stage the "legalLabel" function which is supposed to check whether a 
label has a legal name isn't yet able to check whether the label's name is used in a macro
or a const(defined)

Need to handle receiving lines with too few/many parameters
