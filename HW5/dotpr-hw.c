   #include <pthread.h>
   #define NUMTHRDS 4
   #define VECLEN 100000
   pthread_t callThd[NUMTHRDS];
   double *array_a;
   double *array_b;
   double big_sum;
   int veclen;
   pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;   

typedef struct
{


}thread_info;


void *dotprod(void *arg)
    {
     
     int start, end;
     x = array_a;
     y = array_b;
     double mysum;
     
     start = vectlen + arg
     end = start + arg

     mysum = 0;
     for (i=start; i<end ; i++)
      {
       mysum += (x[i] * y[i]);
      }
      pthread_mutex_lock(&mutex);
      sum += mysum
      pthread_mutex_unlock(&mutex);

    }
   int main (int argc, char *argv[])
    {
     int i;
     double *a, *b;
     void *status;

     thread_info thread_data[NUMTHRDS];

     a = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));
     b = (double*) malloc (NUMTHRDS*VECLEN*sizeof(double));
     for (i=0; i<VECLEN*NUMTHRDS; i++)
      {
       a[i]=1;
       b[i]=a[i];
      }
     veclen = VECLEN;
     array_a = a;
     array_b = b;
     big_sum = 0;
     /* create threads */
     for(i=0;i<NUMTHRDS;i++)
      {
	     pthread_create(&callThd[i], NULL, dotpropd, NULL);
	     pthread_join(callThd[i], status);
      }
	
     printf ("Sum = %f \n", big_sum);
     free (a);
     free (b);
    }
