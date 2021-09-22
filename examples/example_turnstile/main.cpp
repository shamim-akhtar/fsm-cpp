#include "FiniteStateMachine.h"
#include <iostream>
#include <conio.h>
#include <string>

using namespace Patterns;

bool KeyPresses[255];

enum class TurnstileStateType
{
  LOCKED,
  UNLOCKED,
};

class TurnstileLockedState : public State<TurnstileStateType>
{
  FiniteStateMachine<TurnstileStateType>& mFsm;
public:
  TurnstileLockedState(FiniteStateMachine<TurnstileStateType>& fsm)
    : State<TurnstileStateType>(TurnstileStateType::LOCKED, "Locked")
    , mFsm(fsm)
  {
  }

  void enter()
  {
    printf("Turnstile state: LOCKED\n");
  }

  void update()
  {
    if (KeyPresses['c'])
    {
      printf(" - coin inserted, unlocking turnstile\n");
      mFsm.setCurrentState(TurnstileStateType::UNLOCKED);
    }
    else
    {
      printf("Please insert a coin by pressing c to unlock the turnstile.\n");
    }
  }
}; 

class TurnstileUnLockedState : public State<TurnstileStateType>
{
  FiniteStateMachine<TurnstileStateType>& mFsm;
public:
  TurnstileUnLockedState(FiniteStateMachine<TurnstileStateType>& fsm)
    : State<TurnstileStateType>(TurnstileStateType::UNLOCKED, "Unlocked")
    , mFsm(fsm)
  {
  }

  void enter()
  {
    printf("Turnstile state: UNLOCKED\n");
  }

  void update()
  {
    if (KeyPresses['p'])
    {
      printf(" - pushed, locking turnstile\n");
      mFsm.setCurrentState(TurnstileStateType::LOCKED);
    }
  }
};

int main(int argc, char* argv)
{
  printf("--------------------------------------------\n");
  printf("An example demo for the Finite State Machine\n");
  printf("--------------------------------------------\n");
  printf("  Press the c key to insert coin.\n");
  printf("  Press the p key to open the turnstile\n");
  printf("  Press the q key to quit\n");
  printf("--------------------------------------------\n");

  // create the state machine for the turnstile.
  FiniteStateMachine<TurnstileStateType> *fsm = 
    new FiniteStateMachine<TurnstileStateType>();

  fsm->add(new TurnstileLockedState(*fsm));
  fsm->add(new TurnstileUnLockedState(*fsm));

  fsm->setCurrentState(TurnstileStateType::LOCKED);

  bool done = false;
  while (!done)
  {
    char input = _getch();
    KeyPresses[input] = true;
    if (KeyPresses['q'])
    {
      done = true;
      break;
    }

    fsm->update();

    for (int i = 0; i < 255; ++i)
      KeyPresses[i] = false;
  }
  printf("You have exited the program. Good bye!\n");

  delete fsm->getState(TurnstileStateType::UNLOCKED);
  delete fsm->getState(TurnstileStateType::LOCKED);
  delete fsm;
}