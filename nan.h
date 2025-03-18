struct Trace {
	float p;
	float i;
	float d;
	float f_error;
	///@brief -1 left(1), 0 center(2), 1 right(1)
	int type;
	///@brief sikiiti
	int threshold;
};

void initTrace(struct Trace& data, float p,float i, float d) {
	data.p = p;
	data.i = i;
	data.d = d;
	data.f_error = 0;
	data.type = 0;
	data.threshold = 50;
}

void initTrace(struct Trace& data, float p,float i, float d, int type, int threshold) {
	initTrace(data, p, i, d);
	data.type = type;
	data.threshold = threshold;
}

void reset_f_error(struct Trace& data) {
	data.f_error = 0;
}

void tracePID(struct Trace& data, int power) {
	int error,a,b;
	if (data.type == 0)
		error = getCRLR() - getCRLL();
	else if (data.type == -1)
		error = getCRLL() - data.threshold;
	else if (data.type == 1)
		error = data.threshold - getCRLR();

	int p = data.p * error;
	int i = data.i * (error + data.f_error);
	int d = data.d * (error - data.f_error);

	setR(a=power + (p + i + d));
	setL(b=power - (p + i + d));
	data.f_error = error;
}


/*Trace trace;
initTrace(trace, 0.08, 0.035,3.00);//(p, i, d);1konobai( p, i, d,1 or -1,kuro to siro no ave);
clearTimer(T1);
while (1){
tracePID(trace, 30);
displayTextLine(0,"%d",time1[T1]);
if (time1[T1]>960){
break;
clearTimer(T1);
}
}*/
