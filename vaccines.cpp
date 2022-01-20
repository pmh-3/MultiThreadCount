#include <pthread.h>
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

//PROTOTYPE not global variable
//store vaccine counts for each thread
struct rval {
	int sv1 = 0;
	int sv2 = 0;
	int sv3 = 0;
};

//Thread function to count type of vaccines in each file
void *vaxCount(void *arg_id) {
	string line;
	//cast void ptr to int pointer, then dereference
	int id = *(static_cast<int*>(arg_id));

	//cout << "thread ID: " << id << endl;

	//convert int to string for filename
	string sid = to_string(id);

	//local variables
	int tv1, tv2, tv3;
	tv1 = tv2 = tv3 = 0;

	string filePath = "/home/fac/mishraa/cpsc3500/pa3a/city" + sid + ".txt";
	//cout<<filePath<<endl;
	ifstream myfile(filePath);
	if (myfile.is_open())
	{
		while ( getline (myfile,line) )
		{
		  //cout << line << '\n';
		  if(line == "v1"){
			  tv1++;
		  }else if(line == "v2"){
			  tv2++;
		  }else if(line == "v3"){
			  tv3++;
		  }else{
			  cout<< "format ERROR: "<< line << "<< here "<< endl;
		  }
		}
		myfile.close();
	}
	else cout << "Unable to open file";

	//dynamically allocate struct for thread
	rval *ret_p = new rval;
	ret_p->sv1 = tv1; 	//store local data in struct
	ret_p->sv2 = tv2;
	ret_p->sv3 = tv3;

	void pthread_exit(void *ret_p);
	return (void *)ret_p;
}

int main() {

	const int NUM_THREADS = 21;
	int v1,v2,v3;
	v1 = v2 = v3 = 0;
	int t;
	int status;

	pthread_t ID[NUM_THREADS];	//array to store tid

	int arg_arr[NUM_THREADS]; //array to store value of counter

	//loop to create threads
	for(t = 1; t <= NUM_THREADS; t++){

		arg_arr[t] = t; //store VALUE of t

		status = pthread_create(&ID[t], NULL, vaxCount, static_cast<void*>(&arg_arr[t]));

		if(status != 0){
			cout<< "ERROR creating thread " << endl;
			exit(-1);
		}

	}

	//struct ptr to access return values
	rval *val;
	rval **rval_ptr = &val;

	//join loop
	for(t = 1; t <= NUM_THREADS; t++){
		//cout << "Join THREAD: " << t << endl;

		status =  pthread_join(ID[t], (void **) rval_ptr);
		if(status){
				cout << "Error: cannot join " << endl;
				exit(-1);
		}

		//update total
		v1 += (*rval_ptr)->sv1;
		v2 += (*rval_ptr)->sv2;
		v3 += (*rval_ptr)->sv3;

		//reclaim access ptr;
		delete val;

	}

	cout<<"Total Vaccine Count: \n";
	cout<<"v1: " << v1 << endl;
	cout<<"v2: " << v2 << endl;
	cout<<"v3: " << v3 << endl;



	return 0;
}
