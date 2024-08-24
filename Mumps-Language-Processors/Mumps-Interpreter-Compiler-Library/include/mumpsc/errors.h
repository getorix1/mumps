    fprintf( Out, "     if (GlobalExceptionCode == INPUT_ERROR) "
        "cout << \"Input Error\" << endl;\n");
    fprintf( Out, "     else if (GlobalExceptionCode == INPUT_LENGTH) "
        "cout << \"Input Length Error\" << endl;\n");
    fprintf( Out, "     else if (GlobalExceptionCode == SYMTAB_ERROR) "
        "cout << \"Symbol Table Error\" << endl;\n");
    fprintf( Out, "     else if (GlobalExceptionCode == GLOBAL_NOT_FOUND) "
        "cout << \"Global Not Found Error\" << endl;\n");
    fprintf( Out, "     else if (GlobalExceptionCode == INTERPRETER_ERROR) "
        "cout << \"Interprerer Error\" << endl;\n");
    fprintf( Out, "     else if (GlobalExceptionCode == INVALID_CONVERSION) "
        "cout << \"Invalid Conversion Error\" << endl;\n");
    fprintf( Out, "     else if (GlobalExceptionCode == DATA_RANGE_ERROR) "
        "cout << \"Data Range Error\" << endl;\n");
    fprintf( Out, "     else if (GlobalExceptionCode == NUMERIC_RANGE) "
        "cout << \"Numeric Range Error\" << endl;\n");
    fprintf( Out, "     else if (GlobalExceptionCode == UNSPECIFIED) "
        "cout << \"Unspecified Error\" << endl;\n");
