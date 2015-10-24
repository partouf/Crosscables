
#ifndef __GFCALLBACKTHREAD_H
#define __GFCALLBACKTHREAD_H

#include "Thread.h"

#include "../Atoms/Callable.h"
#include "../Molecules/Callbacks.h"

namespace Groundfloor {

   /** CallbackThread is a runnable thread that will call
     * the execute function of a given Callable object every given interval.
     */
   class CallbackThread : public Thread
   {
   protected:
      Callable *aCallableObject;

   public:
      CallbackThread(Callable *anObject, int iSleeptime);
      ~CallbackThread();

      /// Implementation of execute() that calls the execute() member function of another object passed at creation.
      void execute();
   };


   /** CallbackThread is a runnable thread that will call
     * the execute function of the onTimerEvent MultiNotify object every given interval.
     */
   class Timer : public Thread {
   protected:
   public:
      Timer();
      ~Timer();

      MultiNotify<Freeable *> onTimerEvent;

      /// Implementation of execute()
      void execute();
   };


   template <class P>
   class RunnableFunction : public Thread {
   protected:
      typedef void(*FuncCallbackOneParam)(P);

      FuncCallbackOneParam pFunction;
      P aParam;
   public:
      RunnableFunction(FuncCallbackOneParam pFunction) : Thread() {
         this->pFunction = pFunction;

         start();
      }
      RunnableFunction(FuncCallbackOneParam pFunction, P aParam) {
         this->pFunction = pFunction;
         this->aParam = aParam;

         start();
      }
      ~RunnableFunction() {
         stop();
      }

      P getParam() {
         return aParam;
      }

      void runWithParam(P aParam) {
         this->aParam = aParam;

         start();
      }

      void execute() {
         (*pFunction)(aParam);

         stop();
      }
   };

   template <typename P, typename R>
   class RunnableFunctionWithReturn : public Thread {
   protected:
      typedef R(*FuncCallbackOneParam)(P);
      typedef void(*FuncReturnCallbackOneParam)(R);

      FuncCallbackOneParam pFunction;
      FuncReturnCallbackOneParam pReturnFunction;
      P aParam;
      R aReturn;
   public:
      RunnableFunctionWithReturn(FuncCallbackOneParam pFunction, P aParam, FuncReturnCallbackOneParam pReturnFunction) : Thread() {
         this->pFunction = pFunction;
         this->pReturnFunction = pReturnFunction;
         this->aParam = aParam;

         start();
      }
      ~RunnableFunctionWithReturn() {
         stop();
      }

      P getParam() {
         return aParam;
      }

      R getReturnValue() {
         return aReturn;
      }

      void runWithParam(P aParam) {
         this->aParam = aParam;

         start();
      }

      void execute() {
         aReturn = (*pFunction)(aParam);
         if (pReturnFunction != 0) {
            (*pReturnFunction)(aReturn);
         }

         stop();
      }
   };

}

#endif // __GFCALLBACKTHREAD_H
