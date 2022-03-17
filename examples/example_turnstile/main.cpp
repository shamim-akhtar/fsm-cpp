#include "FiniteStateMachine.h"
#include <memory>
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


template <typename T> class Func : public State<T>::Functor
{
  std::string _name;
public:
  explicit Func(std::string name)
    : _name(name)
  {

  }
  void operator()(State<T>& state) override
  {
    std::cout << "Called " + _name << std::endl;
  }
};

class TurnstileLockedState : public State<TurnstileStateType>
{
  FiniteStateMachine<TurnstileStateType>& mFsm;
public:
  TurnstileLockedState(FiniteStateMachine<TurnstileStateType>& fsm)
    : State<TurnstileStateType>(
      TurnstileStateType::LOCKED, 
      "Locked", 
      new Func<TurnstileStateType>("OnEnter"),
      new Func<TurnstileStateType>("OnExit"),
      new Func<TurnstileStateType>("OnUpdate")
      )
    , mFsm(fsm)
  {
  }

  void enter()
  {
    State<TurnstileStateType>::enter();
    printf("Turnstile state: LOCKED\n");
  }

  void update()
  {
    State<TurnstileStateType>::update();
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
    State<TurnstileStateType>::enter();
    printf("Turnstile state: UNLOCKED\n");
  }

  void update()
  {
    State<TurnstileStateType>::update();
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
  std::unique_ptr<FiniteStateMachine<TurnstileStateType>> fsm = std::make_unique<FiniteStateMachine<TurnstileStateType>>();
  std::unique_ptr<TurnstileLockedState> state_locked = std::make_unique<TurnstileLockedState>(*fsm);
  std::unique_ptr<TurnstileUnLockedState> state_unlocked = std::make_unique<TurnstileUnLockedState>(*fsm);

  fsm->add(state_locked.get());
  fsm->add(state_unlocked.get());

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
}