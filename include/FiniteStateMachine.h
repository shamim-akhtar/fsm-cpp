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
      virtual void operator()(State<T>& state)
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
      Functor* onEnter = nullptr,
      Functor* onExit = nullptr,
      Functor* onUpdate = nullptr)
      : mName(name)
      , mID(id)
      , mOnEnter (onEnter)
      , mOnExit(onExit)
      , mOnUpdate(onUpdate)
    {
    }

    virtual ~State() {}

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
      if (state == nullptr)
        return;
      add(state->getID(), state);
      //mStates[state->getID()] = state;
    }

    void add(T stateID, State<T>* state)
    {
      if (mStates.find(stateID) == mStates.end())
      {
        mStates.insert(std::make_pair(stateID, state));
      }
      else
      {
        assert(0);
      }
    }

    State<T>* getState(T stateID)
    {
      if (mStates.find(stateID) != mStates.end())
        return mStates[stateID];
      return nullptr;
    }

    State<T>* getCurrentState()
    {
      return mCurrentState;
    }

    const State<T>* getCurrentState() const
    {
      return mCurrentState;
    }
    
    void setCurrentState(T stateID)
    {
      State<T>* state = getState(stateID);
      assert(state != nullptr);
      if (state != nullptr)
      {
        setCurrentState(state);
      }
    }

    void setCurrentState(State<T>* state)
    {
      if (mCurrentState == state)
      {
        return;
      }

      if (mCurrentState != nullptr)
      {
        mCurrentState->exit();
      }

      mCurrentState = state;

      if (mCurrentState != nullptr)
      {
        mCurrentState->enter();
      }
    }

    void update()
    {
      if (mCurrentState != nullptr)
      {
        mCurrentState->update();
      }
    }
  };
}

