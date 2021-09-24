/*
 * Copyright 2007-2021 United States Government as represented by the
 * Administrator of The National Aeronautics and Space Administration.
 * No copyright is claimed in the United States under Title 17, U.S. Code.
 * All Rights Reserved.
 */







/** @file ExecutorService.h
 *
 *  @brief This file contains a cross-platform ExecutorService that functions the same
 *	as the Java class of the same name.
**/

#ifndef gmsec_util_ExecutorService_h
#define gmsec_util_ExecutorService_h

namespace gmsec
{
namespace internal
{
	template <class elemType>
	class Executor : public gmsec::util::Thread
	{
	public:

		Executor(int id) : alive(true), cdl(1){
			ident = id;
			active = false;
			current = NULL;
		}
		~Executor(){

		}
		void CALL_TYPE run(){
			while(alive.get()){
				{
					AutoMutex hold(cond.getMutex());
					cond.Wait();
					active = true;
				}
				current->execute();
				{
					AutoMutex hold(cond.getMutex());
					active = false;
				}
			}
			cdl.countDown();
		}
		bool isActive(){
			AutoMutex hold(cond.getMutex());
			return active;
		}
		void terminate(){
			alive.set(false);
			cdl.await();
		}
		bool accept(gmsec::util::Future<elemType> *future){
			AutoMutex hold(cond.getMutex());
			if(active){
				return false;
			}
			current = future;
			
			cond.Signal(Condition::USER);
			return true;
		}

	private:

		Executor(const Executor &);
		Executor &operator=(const Executor &);

		int ident;
		gmsec::util::Future<elemType> *current;
		gmsec::util::Condition cond;
		bool active;
		AtomicBoolean alive;
		gmsec::util::CountDownLatch cdl;

	};


template <class elemType>
class ExecutorService : public gmsec::util::Thread
{
public:

	ExecutorService(int c = 1) : aBool(true), cdl(1), queue(1000), executors(c)
	{
		count = c;
		for(unsigned int i = 0; i < executors.size(); i++){
			executors[i] = new Executor<elemType>(i);
			executors[i]->start();
		}
	}

	~ExecutorService()
	{

	}

	void process(gmsec::util::Future<elemType> *future)
	{
		queue.put(future);
	}

	void shutdown()
	{
		aBool.set(false);
		cdl.await();
		for(int i = 0; i < executors.size(); i++){
			executors[i]->terminate();
		}
	}

	void CALL_TYPE run()
	{
		while(aBool.get()){
			Future<elemType> *current;
			if(queue.poll(1000, current)){
				if(count == 1){
					current->execute();
				}else{
					bool loop = true;
					while(loop){
						for(unsigned int i = 0; i < executors.size(); i++){
							if(!executors[i]->isActive()){
								if(executors[i]->accept(current)){
									loop = false;
									break;
								}
							}
						}
						Thread::yield();
					}
				}
			}
		}
		cdl.countDown();
	}

private:

	// Declared, but not implemented.
	ExecutorService(const ExecutorService &);
	ExecutorService &operator=(const ExecutorService &);

	int count;
	gmsec::util::AtomicBoolean aBool;
	gmsec::util::CountDownLatch cdl;
	gmsec::util::BoundedQueue<gmsec::util::Future<elemType> *> queue;
	gmsec::util::Condition condition;
	std::vector<gmsec::util::Executor<elemType> *> executors;

};

}
}

#endif

