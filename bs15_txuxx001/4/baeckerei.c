#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;			//init mutex
pthread_cond_t lieferantCond, backerCond;				// create two condition variables
int eier, milch, mehl,bleche=0;						// counters for materials

void* startbacken(){
	int ret;
	while(bleche < 10){						//10 Bleche need to be toasted
		ret = pthread_mutex_lock(&mutex);						
		assert( 0 == ret);
		while(5 != eier || 1 != milch || 2 != mehl){						//check, if everything ready
													//as not, baecker must wait
			printf("			Baecker: es fehlen noch Zutaten,warte...\n" );
			ret = pthread_cond_wait(&backerCond,&mutex);					// baecker is blocked
			assert( 0 == ret);					
		}


		while(5 == eier && 1 == milch && 2 == mehl){					//as everything ok,start to backen
			printf("			Baecker: gerade backen...\n" );
					sleep(5);										//backen for 5 mins
					bleche++;	
					printf("			Baecker: %d mal Bleche schon fertig\n",bleche ); 	//output how many already finished				
					eier = 0;
					milch = 0;										
					mehl = 0;				// one Bleche finished, all of material are used then deliver again					
				}
				ret = pthread_cond_broadcast(&lieferantCond);			// wake up Lieferanten
				assert( 0 == ret);
				printf("			Baecker: Backstube verlassen\n" );
				ret = pthread_mutex_unlock(&mutex);
				assert( 0 == ret);
			}

			pthread_exit(NULL);
		}

		void* starteierliefern(){
			int ret;
			
			while(bleche < 10){				
				sleep(1);										// egg to be delivered every one minute
				ret = pthread_mutex_lock(&mutex);
				assert( 0 == ret);
				while(5 == eier ){					
					ret = pthread_cond_wait(&lieferantCond,&mutex);		// as already 5 eggs, blocked
					assert( 0 == ret);
				}
				if(10 == bleche){									//as already 10 Bleche, finish this thread
					printf("Alle schon fertig gemacht, 88\n");
					ret = pthread_mutex_unlock(&mutex);				//before thread finish, unlock the critical region
					assert( 0 == ret);
					pthread_exit(NULL);								//finish this thread
				}
				eier++;												// one more egg is delivered
				printf("Eier:Anlieferung\n");
				printf("Eier:es sind %d Ei(er) in der Backstube\n",eier );
				printf("Eier:Backstube verlassen\n" );	

				if(5 == eier){										//as already 5 eggs, wake up the Baecker
					printf("Eier: wecke Baecker\n");
					printf("Eier:Backstube verlassen\n" );	
					ret = pthread_cond_signal(&backerCond);			//send the signal to the Baecker
					assert( 0 == ret);
				}
				
				ret = pthread_mutex_unlock(&mutex);
				assert( 0 == ret);				
			}

			pthread_exit(NULL);
		}

		void* startmilchliefern(){
			int ret;
			while(bleche < 10){				
				sleep(2);										// milk to be delivered every two minutes
				ret = pthread_mutex_lock(&mutex);
				assert( 0 == ret);
				
				while(milch == 1){										// as already 1 liter milk, blocked
					ret = pthread_cond_wait(&lieferantCond,&mutex);	
					assert( 0 == ret);
				}												
				if(10 == bleche){									//as already 10 Bleche, finish this thread
					ret = pthread_mutex_unlock(&mutex);
					assert( 0 == ret);
					pthread_exit(NULL);
				}

				milch++;											// one more milk is delivered
				printf("	Milch:Anlieferung\n");
				printf("	Milch:es sind %d Liter Milch in der Backstube\n",milch );	
				printf("	Milch: Backstube verlassen\n");	

				if(milch == 1){										//as already 1 liter milk, wake up the Baecker
					printf("	Milch: wecke Baecker\n");
					printf("	Milch: Backstube verlassen\n");	
					ret = pthread_cond_signal(&backerCond);			//wake up the Baecker to check, if everything ok
					assert( 0 == ret);
					ret = pthread_mutex_unlock(&mutex);
					assert( 0 == ret);
				}
			}
			pthread_exit(NULL);
		}

		void* startmehlliefern(){
			int ret;

			while(bleche < 10){					
				sleep(3);											//flout to be delivered every three minutes
				ret = pthread_mutex_lock(&mutex);	
				assert( 0 == ret);
				while(mehl == 2){									// as already 2 flour, blocked and unlock
					ret = pthread_cond_wait(&lieferantCond,&mutex);	
					assert( 0 == ret);
				}
				if(10 == bleche){									//as already 10 Bleche, finish this thread
					ret = pthread_mutex_unlock(&mutex);
					assert( 0 == ret);
					pthread_exit(NULL);
				}
				mehl++;												// one more flour is delivered
				printf("		Mehl:Anlieferung\n");
				printf("		Mehl:es sind %d kg Mehl in der Backstube\n",mehl );
				printf("		Mehl:Backstube verlassen\n");	

				if(mehl == 2){										//as already 2 flour, wake up the Baecker
					printf("		Mehl:wecke Baecker\n");
					printf("		Mehl:Backstube verlassen\n");	
					ret = pthread_cond_signal(&backerCond);			// send the signal to the Baecker
					assert( 0 == ret);
				}
				ret = pthread_mutex_unlock(&mutex);
				assert( 0 == ret);
			}	

			pthread_exit(NULL);
		}

		int main(int argc, char *argv[])
		{
			printf("Plaetzchenbaeckerei ...\n\n");
			pthread_t eierLiefer, milchLiefer,mehlLiefer, backen;	// create threads
			int ret;
			pthread_attr_t attr[4];
			int i;
			for(i=0;i<4;i++){
				ret = pthread_attr_init(&attr[i]);					// init attr
				assert(0 == ret);
			}
			assert(0 == ret);										// init threads
			ret = pthread_create(&backen,&attr[0],startbacken,0);
			assert(0 == ret);
			ret = pthread_create(&eierLiefer,&attr[1],starteierliefern,0);
			assert(0 == ret);
			ret = pthread_create(&milchLiefer,&attr[2],startmilchliefern,0);
			assert(0 == ret);
			ret = pthread_create(&mehlLiefer,&attr[3],startmehlliefern,0);
			assert(0 == ret);

			ret = pthread_join(backen,NULL);					// wait for all of threads finish
			assert( 0 == ret);
			ret = pthread_join(eierLiefer,NULL);
			assert( 0 == ret);
			ret = pthread_join(milchLiefer,NULL);
			assert( 0 == ret);
			ret = pthread_join(mehlLiefer,NULL);
			assert( 0 == ret);
																// destroy two condition variables
			ret = pthread_cond_destroy(&lieferantCond);
			assert( 0 == ret);
			ret = pthread_cond_destroy(&backerCond);
			assert( 0 == ret);
																//destroy mutex
			ret = pthread_mutex_destroy(&mutex);
			assert(0 == ret);

			return 0;
		}

