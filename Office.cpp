#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
#include "GenQueue.h"
#include "Window.h"

using namespace std;

int main(int argc, char *argv[]){
  GenQueue<Student*> queue;
  ifstream input(argv[1]);
  int numStudents = 0;
  int numWindows = 0;

  //math vars
  int totalWait = 0;
  int totalIdle = 0;
  float meanWait = 0;
  float meanIdle = 0;
  int medianWait = 0;
  int maxWait = 0;
  int maxIdle = 0;
  int numWinIdle = 0;
  int numStudWait = 0;

  //tracker
  int time = 0;
  string line = "";
  GenQueue<int> waitTimes;

  getline(input,line);
  numWindows = stoi(line);
  Window** windows = new Window*[numWindows];
  for(int i = 0; i<numWindows; i++){
    Window* temp = new Window();
    windows[i] = temp;
  }

  while(getline(input,line)){
    time++;
    getline(input,line);
    numStudents = numStudents + stoi(line);
    int tempStudents = stoi(line);
    Student *s;

    for(int i = 1; i<=tempStudents; i++){
      getline(input,line);
      s = new Student(stoi(line));
      queue.insert(s);
    }

    for(int i = 0; i<numWindows; i++){
      if(!queue.isEmpty()){
        windows[i]->setStudent(queue.remove());
      }else{
        break;
      }
    }

    //queue holds all students wait times

    //this insert 0 ot lal places
    int currWait = 0;
    bool y = false;
    if(tempStudents<numWindows){
      for(int i = 1; i<=tempStudents; i++){
        waitTimes.insert(0);
      }
    }else{
      for(int i = 1; i<=numWindows; i++){
        waitTimes.insert(0);
      }
    }

    while(!queue.isEmpty()){
      y = true;
      for(int i = 0; i<numWindows; i++){
        if(windows[i]->getStudent()->getTimeTaken() == 0){
          if(!queue.isEmpty()){
            windows[i]->setStudent(queue.remove());
            windows[i]->getStudent()->decrementTime();
            totalWait = totalWait + currWait;
            waitTimes.insert(currWait);
            if(currWait>=10){
              numStudWait++;
            }
          }else{
            windows[i]->setStudent(NULL);
          }
        }else{
          windows[i]->getStudent()->decrementTime();
        }
      }
      currWait++;
    }
    if(maxWait<currWait-1){
      maxWait = currWait-1;
    }

    int x = 0;
    for(int i = 0; i<numWindows; i++){
      if(!windows[i]->isIdle()){
        if(x<windows[i]->getStudent()->getTimeTaken()){
          x = windows[i]->getStudent()->getTimeTaken();
        }
      }
    }

    for(int i = 0; i<numWindows; i++){
      if(!windows[i]->isIdle()){
        windows[i]->setTotalIdle(windows[i]->getTotalIdle() + (x-windows[i]->getStudent()->getTimeTaken()));
        windows[i]->setCurrIdle(x-windows[i]->getStudent()->getTimeTaken());
      }else if(y){
        windows[i]->setTotalIdle(windows[i]->getTotalIdle() + x + 1);
        windows[i]->setCurrIdle(x + 1);
      }else{
        windows[i]->setTotalIdle(windows[i]->getTotalIdle() + x);
        windows[i]->setCurrIdle(x);
      }
    }

    for(int i = 0; i<numWindows; i++){
      if(maxIdle < windows[i]->getCurrIdle()){
        maxIdle = windows[i]->getCurrIdle();
      }
    }
  }

  //calculate mean wait time
  meanWait = (totalWait*1.0)/numStudents;

  //calculate number of windows idle for 5
  for(int i = 0; i<numWindows; i++){
    totalIdle = totalIdle + windows[i]->getTotalIdle();
    if(5 <= windows[i]->getTotalIdle()){
      numWinIdle++;
    }
  }

  //calculate mean idle time
  meanIdle = totalIdle/(numWindows*time*1.0);


  int array[numStudents];
  for(int i = 0; i<numStudents; i++){
    array[i] = waitTimes.remove();
  }
  sort(array,array+numStudents);
  if(numStudents%2 == 0){
    medianWait = (array[numStudents/2] + array[(numStudents/2) + 1])/2;
  }else{
    medianWait = array[(numStudents/2) + 1];
  }

  cout<<"Mean student wait time: "<<meanWait<<endl;
  cout<<"Median student wait time: "<< medianWait<<endl;
  cout<<"Longest student wait time: "<< maxWait<<endl;
  cout<<"Number of students waiting over 10: "<<numStudWait<<endl;
  cout<<"Mean window idle time: "<< meanIdle<<endl;
  cout<<"Longest window idle time: "<< maxIdle<<endl;
  cout<<"Number of windows idle over 5: "<<numWinIdle<<endl;

  return 0;
}
