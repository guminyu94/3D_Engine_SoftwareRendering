#pragma once


class box
{
public:
	box(float _length)
		:length(_length),
		vertex_cor({ 
			{length / 2,-length / 2,-length / 2},{length / 2,length / 2,-length / 2},
	{-length / 2,length / 2,-length / 2},{-length / 2,-length / 2,-length / 2},
	{length / 2,length / 2,length / 2},{-length / 2,length / 2,length / 2},
	{-length / 2,-length / 2,length / 2},{length / 2,-length / 2,length / 2} 
			})
	{}

private:
	float length;
	const float vertex_cor[8][3];

};
