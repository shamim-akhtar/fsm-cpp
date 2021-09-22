#pragma once
#include <vector>
#include <map>
#include <string>
#include <cassert>

namespace Patterns
{
  template <typename T>
  class State
  {
  public:
    // A functor for callbacks 
    class Functor
    {
    public:
      virtual ~Functor() {}
      template<typename T> void operator()(State<T>& state)
      {
      }
    };

  public:
    // The ID of the state.
    inline T getID()
    {
      return mID;
    }

    inline const std::string& getName() const
    {
      return mName;
    }

    explicit State(T id,
      std::string name = "default",
      Functor* onEnter = 0,
      Functor* onExit = 0,
      Functor* onUpdate = 0) 
      : mName(name)
      , mID(id)
      , mOnEnter (onEnter)
      , mOnExit(onExit)
      , mOnUpdate(onUpdate)
    {
    }

    virtual void enter()
    {
      if (mOnEnter)
      {
        (*mOnEnter)(*this);
      }
    }

    virtual void exit()
    {
      if (mOnExit)
      {
        (*mOnExit)(*this);
      }
    }
    virtual void update()
    {
      if (mOnUpdate)
      {
        (*mOnUpdate)(*this);
      }
    }
  private:
    std::string mName;
    T mID;

    Functor* mOnEnter;
    Functor* mOnExit;
    Functor* mOnUpdate;
  };

  template <typename T>
  class FiniteStateMachine
  {
  // A Finite State Machine
  // - An FSM consists of a set of states,
  // - and at any given time, an FSM can exist in only one 
  //   State out of these possible set of states.

  // A map to represent the a set of states.
  protected:
    std::map<T, State<T>*> mStates;

    // The current state.
    State<T>* mCurrentState;

  public:
    FiniteStateMachine()
      : mCurrentState(0)
    {
    }

    void add(State<T> *state)
    {
      if (state == 0)
        return;
      mStates[state->getID()] = state;
    }

    void add(T stateID, State<T>* state)
    {
      mStates.add(stateID, state);
    }

    State<T>* getState(T stateID)
    {
      return mStates[stateID];
    }

    State<T>* getCurrentState()
    {
      return mCurrentState;
    }

    void setCurrentState(T stateID)
    {
      State<T>* state = getState(stateID);
      assert(state != 0);

      setCurrentState(state);
    }

    void setCurrentState(State<T>* state)
    {
      if (mCurrentState == state)
      {
        return;
      }

      if (mCurrentState != 0)
      {
        mCurrentState->exit();
      }

      mCurrentState = state;

      if (mCurrentState != 0)
      {
        mCurrentState->enter();
      }
    }

    void update()
    {
      if (mCurrentState != 0)
      {
        mCurrentState->update();
      }
    }
  };
}

