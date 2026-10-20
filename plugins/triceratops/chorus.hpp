
class chorus
{
	public:

	chorus();
	~chorus();
	float* run(float, float);
	
	int length = 256;
	float main_output[2];
	float buffer_left_in[4096];
	float buffer_right_in[4096];
	int buffer_position;
		                                                                                                                                                                                                                                                                      
	float buffer_right_out[4096];
	float buffer_left_out[4096];

	float frequency_one;
	float frequency_one_inc;
	float index_one;

	float frequency_two;
	float frequency_two_inc;
	float index_two;
};




