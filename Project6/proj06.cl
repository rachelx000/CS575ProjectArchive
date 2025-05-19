#define IN
#define OUT

kernel
void
Regression(	IN global const float *dX,
	   	IN global const float *dY,
		OUT global float *dSumx4,
		OUT global float *dSumx3,
		OUT global float *dSumx2,
		OUT global float *dSumx,
		OUT global float *dSumx2y,
		OUT global float *dSumxy,
		OUT global float *dSumy )
{
	int gid = get_global_id( 0 );
	// int numItems = get_local_size( 0 );     // local work-group size
	// int tnum     = get_local_id( 0 );       // local thread number
	// int wgNum    = get_group_id( 0 );       // global work-group number

	float x = dX[gid];
	float y = dY[gid];
	dSumx4[ gid ]  = x * x * x * x;
	dSumx3[ gid ]  = x * x * x;
	dSumx2[ gid ]  = x * x;
	dSumx[ gid ]   = x;
	dSumx2y[ gid ] = x * x * y;
	dSumxy[ gid ]  = x * y;
	dSumy[ gid ]   = y;
}
