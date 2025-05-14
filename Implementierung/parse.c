#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <time.h>
#include "read.h"
#include "parse.h"

// Helper function to parse floating-point values for options
void strtof1(char* optarg, char* endptr, const char* option, float* arg, int cases ) {
   

    if (optarg == NULL) {
       
            fprintf(stderr, "Error: Missing argument for option --%s. Expected 3 floats.\n", option);        //Covering the case if the arguments of the option --coeffs arent set 
            exit(EXIT_FAILURE);
           
    } else {
        float result = strtof(optarg, &endptr);  //Doing the conversion

        if (*endptr != '\0' || endptr == optarg) {
            if(cases==0){
            fprintf(stderr, "Error: Invalid argument for option --%s. Expected a float.\n", option);       //Covering the case if the arguments of the option --gamma arent set right
            exit(EXIT_FAILURE);  
        }else{
            fprintf(stderr, "Error: Invalid argument for option --%s. Expected 3 floats.\n", option);     //Covering the case if the arguments of the option --coeffs arent set right
            exit(EXIT_FAILURE);
        }      
        }

        if (result < 0) {
            fprintf(stderr, "Error: The argument for the option --%s can't be negative!\n", option);       //Covering negative inputs for both options
            exit(EXIT_FAILURE);
        }

        *arg = result;   
        return;
    }
}
// Helper function to parse integer values for options
void strtol1(char* optarg, char* endptr, const char* option, uint32_t* arg) {
    endptr = NULL;

    if (optarg == NULL) {

        return;
    } else {
        int result = strtol(optarg, &endptr, 10); //Doing the conversion

        if (*endptr != '\0' || endptr == optarg) {
            fprintf(stderr, "Error: Invalid argument for option -%s. Expected an integer.\n", option);  //Covering the case if the arguments forf the options -V or -B arent set 
            exit(EXIT_FAILURE);
        }

        if (result < 0) {
            fprintf(stderr, "Error: The argument for the option -%s can't be negative!\n", option); //Covering negative inputs for both options
            exit(EXIT_FAILURE);
        }

        *arg = (uint32_t)result;
        return;
    }
}
// Main parsing function
void parse(struct arg *parser,int argc, char **argv){
static struct option long_options[] = {                       //Long options to use in getopt_long
        {"coeffs", required_argument, NULL, 'c'},
        {"gamma", required_argument, NULL, 'g'},
        {"help", no_argument, NULL, 'h'},
        {0, 0, 0, 0}
    };

    char* endptr=NULL;
    int opt = 0;
    
    while ((opt = getopt_long(argc, argv, "V::B::o:i:h",long_options,NULL)) != -1)
    {
        switch (opt)
        {   // Help information
            case 'h':        
            printf("Help \n");
            printf("Options and their functions: \n");
            printf("-V<number>: Used to specify the implementation chosen to be executed. <number> should be chosen from 0-4, corresponding to the names of the implementations. If this option is not set, the main implementation is used by default. \n");
            printf("-B<number>: If explicitly written, the runtime of the specified implementation will be measured and displayed in the console. <number> specifies the number of function call repetitions. \n");
            printf("-o<Dateiname>: Used to specify the output file name. \n");
            printf("—coeffs<FP Zahl>, <FP Zahl>, <FP Zahl>: Used to set the coefficients a, b and c to realise the grayscale conversion.If this option is not set, default values are used. \n");
            printf("—gamma<Floating Point Zahl>: Used to set the gamma value for gamma correction. This value must be non negative. The most common value is 2.2 according to the latest resolution of modern monitors. If the gamma value is bigger than 1, the output file will appear darker. Otherwise, it will appear lighter. \n");
            printf("\n");
            printf("Positional arguments: \n");
            printf("-<Dateiname>: Used to specify the input file to be processed. \n");
            exit(0);
            break;
            case 'V':
            // Parse and assign the value for the -V option
            char * option="V";
            strtol1(optarg,endptr,option,&parser->V);               
            break;
            case 'B':
            // Parse and assign the value for the -B option
            char * option1="B";
            strtol1(optarg, endptr,option1,&parser->B);
            break;
            case 'o':
            // Assign the value for the -o option
            parser->o=optarg;
            break;
            case'g':
            // Parse and assign the value for the --gamma option
            char * option2="gamma";
            strtof1(optarg,endptr,option2,&parser->gamma,0);
            break;
            case 'c': 
             // Parse and assign the values for the --coeffs option
            char * option3="coeffs";                              //Getting the 3 arguments of the option --coeffs
            strtof1(optarg,endptr,option3,&parser->c1,1);
            strtof1(argv[optind++],endptr,option3,&parser->c2,1);
            strtof1(argv[optind++],endptr,option3,&parser->c3,1);
            break;
            default:
            printf("Wrong argument is being pasted.\n");
                //Unknown argument 
                exit(EXIT_FAILURE);
                break;
        }
    }
    if (optind >= argc) {
         
        fprintf(stderr, "No file given\n");          // Checking for the file
        exit(EXIT_FAILURE);
    }
    PPMImage image_data = read_p6(argv[optind]);
            parser->image = image_data.image;
            parser->height = image_data.height;
            parser->width = image_data.width;      //Getting the data from the file
    
    if (!parser->image) {
         free(parser->image);
        exit(EXIT_FAILURE);
    }
}
