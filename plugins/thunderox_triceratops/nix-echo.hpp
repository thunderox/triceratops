
#include <vector>
#include <cmath>
#include <iostream>
#include "eq.h"

class nixecho
{

public:
	nixecho();
	~nixecho();
	float do_left(float);
	float do_right(float);
	void clear_buffer();
	void set_sample_rate(float);
	void reset();
	int echo_speed;
	float echo_decay;
	EQSTATE* eq;
	EQSTATE* eq2;

	float feedback_left,feedback_right;

private:

	std::vector <float> buffer_left,buffer_right;

	int play_head,stereo_width;
	int delay_left,delay_right;
	int old_in_left,old_in_right;

};





