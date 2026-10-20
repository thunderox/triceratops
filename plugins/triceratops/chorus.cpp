
#include "chorus.hpp"
using namespace std;

chorus::chorus()
{
	length = 256;
	frequency_one = 1;
	frequency_one_inc = 0.000001;
	frequency_two = 1;
	frequency_two_inc = 0.0000002;
	
	buffer_position = 0;
	index_one = 0;

	for (int x=0; x<length; x++)
	{
		buffer_left_in[x] = 0;
		buffer_right_in[x] = 0;
		buffer_left_out[x] = 0;
		buffer_right_out[x] = 0;
	}
}

chorus::~chorus()
{
}

float* chorus::run(float left, float right)
{
	float out_left, out_right;

	buffer_left_in[buffer_position] = left;
	buffer_right_in[buffer_position] = right;	

	buffer_position++;
	if (buffer_position > length-1) buffer_position = 0;

	//---------------

	float result = 0;
	int index_one_int = index_one;

	if (index_one == index_one_int) 
	{
		result = buffer_left_in[index_one_int];		
	}
	else
	{
		float fraction1 = index_one - index_one_int;
		float fraction2 = 1 - fraction1;


		if (index_one < length-2)
		{	

			result = buffer_left_in[index_one_int] * fraction2;
			result += buffer_left_in[index_one_int+1] * fraction1;
		}
		else 
		{
			result = buffer_left_in[index_one_int] * fraction2;
			result += buffer_left_in[0] * fraction1;
		}
	}

	frequency_one += frequency_one_inc;

	if (frequency_one > 1.01) frequency_one_inc = -0.000003;
	if (frequency_one < 0.99) frequency_one_inc = 0.000003;

	index_one += frequency_one;
	if (index_one > length - 1 ) index_one -= length;

	out_left = result;

	//---------------

	result = 0;
	int index_two_int = index_two;

	if (index_two == index_two_int) 
	{
		result = buffer_left_in[index_two_int];		
	}
	else
	{
		float fraction1 = index_two - index_two_int;
		float fraction2 = 1 - fraction1;


		if (index_two < length-2)
		{	

			result = buffer_left_in[index_two_int] * fraction2;
			result += buffer_left_in[index_two_int+1] * fraction1;
		}
		else 
		{
			result = buffer_left_in[index_two_int] * fraction2;
			result += buffer_left_in[0] * fraction1;
		}
	}

	frequency_two += frequency_two_inc;

	if (frequency_two > 1.01) frequency_two_inc = -0.000002;
	if (frequency_two < 0.99) frequency_two_inc =  0.000002;

	index_two += frequency_two;
	if (index_two > length - 1 ) index_two -= length;

	out_right = result;
	main_output[0] = (left*0.6) + ((out_left + out_right) * 0.3);
	main_output[1] = (right*0.6) + ((out_left + out_right) * 0.3);

	return main_output;
}
