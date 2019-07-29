#include <stdio.h>

typedef unsigned long DATA;
typedef unsigned int SETTING;
typedef signed char STATUS;

STATUS Repeated_Encoding_1bit_Error(DATA *Input_data, DATA *Output_data, SETTING Payload_length);
STATUS Error_Injection(DATA *Input_data, DATA *Output_data, SETTING Valid_length, DATA Error_mask);
STATUS Repeated_Decoding_1bit_Error(DATA *Input_data, DATA *Output_data, SETTING Payload_length);


STATUS Repeated_Encoding_1bit_Error(DATA *Input_data, DATA *Output_data, SETTING Payload_length)
{
    if ((Payload_length > (SETTING)8) || (Payload_length < (SETTING)1))
        return -1;
    printf("%d\n%d\n", *Input_data, *Output_data);
    *Output_data = ((*Input_data) << (Payload_length << 1)) + ((*Input_data) << Payload_length) + *Input_data;
    
    return 0;
}

STATUS Error_Injection(DATA *Input_data, DATA *Output_data, SETTING Valid_length, DATA Error_mask)
{
    SETTING bit_no;

    if (Valid_length > (SETTING)32)
        return -1;

    for (bit_no = 31; bit_no >= 0; bit_no--)
    {
        if (Error_mask & (1 << bit_no))
        {
            *Output_data = (*Input_data & ~(1 << bit_no)) + (~(*Input_data) & (1 << bit_no));
        }
    } 
    
    return 0;

}

STATUS Repeated_Decoding_1bit_Error(DATA *Input_data, DATA *Output_data, SETTING Payload_length)
{
    DATA temp_data;
    SETTING payload_mask, hamming, previous_hamming, bit_no_in_paylaod, loop;

    if ((Payload_length > (SETTING)8) || (Payload_length < (SETTING)1))
        return -1;

    for (payload_mask = 0; (payload_mask << Payload_length) - (SETTING)1; payload_mask++) 
    {
        hamming = 0;
        for(loop = 0; loop < 3; loop++)
        {
            temp_data = (*Input_data << (loop * Payload_length)) >> (2 * Payload_length); /* MSB ---> LSB */
            
            for (bit_no_in_paylaod = 0; bit_no_in_paylaod < Payload_length; bit_no_in_paylaod++)
            {
                if ((temp_data & (1 << bit_no_in_paylaod)) != (payload_mask & ( 1 << bit_no_in_paylaod)))
                    hamming++;
            }
        }
        if ((previous_hamming == -1) || (hamming < previous_hamming))
            previous_hamming = hamming;
    }
    
    if (hamming != 1)
        return -2;

    return 0;
}

STATUS main()
{
    DATA *Original_data;
    DATA *Encoded_data;
    DATA data = 2;
    SETTING Payload_length = 3;
    STATUS status;
 
    Original_data = &data;
    status = Repeated_Encoding_1bit_Error(Original_data, Encoded_data, Payload_length);

    if (status != 0)
        printf("Encoding Wrong\n"); 
    else
        printf("Encoding Result = %d\n", *Encoded_data);

#if 0   
    Error_Injection(DATA *Input_data, DATA *Output_data, SETTING Valid_length, DATA Error_mask);
    Repeated_Decoding_1bit_Error(DATA *Input_data, DATA *Output_data, SETTING Payload_length);
#endif
    return 0;
}
