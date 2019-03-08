//TASK 1 using FSM Architecture
#include<bits/stdc++.h>
using namespace std;
class FSM
{
	public: int state,n,s,f,up,down,min,mind,minu,max,maxd,maxu,end,path,pathway;//first_choice;
	int *stop;
	public: FSM()						//path1 for down,up,down
	{							//path2 for up,down,up
		int x,i;
		cout<<"Enter number of floors\n";
		cin>>n;
		stop=(int *)malloc(n*sizeof(int));		//if stop[i]==0 then no need to stop at floor i+1,if stop[i]==1, then stop only on going up,if stop[i]==-1 then stop only going down, if stop[i]==2 then stop either way
		for(i=0;i<n;i++)
		stop[i]=0;
		cout<<"Enter start floor\n";
		cin>>s;
		min=s;
		max=s;
		cout<<"Enter number of floor buttons from inside the lift\n";
		cin>>f;
		cout<<"Enter the floor buttons\n";
		for(i=0;i<f;i++)
		{
			cin>>x;                           //finding the maximum and minimum of the floor buttons entered
			if(x>max)
				max=x;
			if(x<min)
				min=x;
			stop[x-1]=2;                      //initializing stop corresponding to floor buttons to 2 indicating that the lift can stop on a floor corresponding to the floor button irrespective of the direction
		}
		cout<<"Enter number of call buttons for moving up\n";
		cin>>up;
		cout<<"Enter the call buttons for moving up \n";
		for(i=0;i<up;i++)
		{
			cin>>x;
			if(x>max)                         //finding the maximum and minimum among call buttons and the floor buttons
				max=x;
			if(x<min)
				min=x;
			stop[x-1]=1;                      //initializing stop corresponding to call button as 1 to indicate that the lift can stop here only while moving up
		}
		cout<<"Enter number of call buttons for moving down\n";
		cin>>down;
		cout<<"Enter the call buttons for moving down \n";
		for(i=0;i<down;i++)
		{
			cin>>x;
			if(x>max)
				max=x;
			if(x<min)
				min=x;
			stop[x-1]=-1;                    //initializing stop corresponding to call button as -1 to indicate that the lift can stop here only while going down
		}
		stop[max-1]=2;					//for max and min stop floors call buttons, direction doesn't matter
		stop[min-1]=2;
	}
	public: void start()
	{
		state=s;                                 //state shows the current floor (basic principle of fsm)
	}
	public: void compute_path()
	{
		//cout<<"compute path\n";
		int path1=0,path2=0,l,m,pos=s-1,end1,end2;  //stop1 is an an array that stores all the stop variables of all floors for path 1 and similarly for stop2 stores these variables for path 2
		int stop1[n],stop2[n];
		for(l=0;l<n;l++)
		{
			stop1[l]=stop[l];   //initializing all stop1 and stop2 variables to the stop variables of the corresponding floors
			stop2[l]=stop[l];
		}
		////PATH 1////
 		for(l=s-1;l>=min-1;l--) //going down from initial floor
		{
			if((stop1[l]==2) or (stop1[l]==-1))   //lift will stop only at those floors whose stop variables are 2 or -1
			{
				stop1[l]=0;    //initializing those stop variables to zero so that the lift doesn't stop there again
				path1=path1+abs(pos-l);    //computing the path length
				pos=l;
			}
		}
		for(l=min-1;l<=max-1;l++)//going up
		{
			if((stop1[l]==2) or (stop1[l]==1))  //lift will stop only at those floors whose stop variables are 2 or 1
			{
				stop1[l]=0;
				path1=path1+abs(pos-l);   //computing the path length
				pos=l;
			}
		}
		for(l=max-1;l>=s-1;l--)// going down again
		{
			if((stop1[l]==2) or (stop1[l]==-1))//lift will stop only at those floors whose stop variables are 2 or -1
			{
				stop1[l]=0;
				path1=path1+abs(pos-l);   //computing the path length
				pos=l;
			}
		}
		end1=pos+1;
		////PATH 2///
		pos=s-1;
		for(l=s-1;l<=max-1;l++)  //going up from initial floor
		{
			if((stop2[l]==2) or (stop2[l]==1)) //lift will stop only at those floors whose stop variables are 2 or 1
			{
				stop2[l]=0;
				path2=path2+abs(pos-l);   //computing the path length
				pos=l;
			}
		}
		for(l=max-1;l>=min-1;l--) //going down                 
		{
			if((stop2[l]==2) or (stop2[l]==-1)) //lift will stop only at those floors whose stop variables are 2 or -1
			{
				stop2[l]=0;
				path2=path2+abs(pos-l);  //computing the path length
				pos=l;
			}
		}
		for(l=min-1;l<=s-1;l++) //going up
		{
			if((stop2[l]==2) or (stop2[l]==1))  //lift will stop only at those floors whose stop variables are 2 or 1
			{
				stop2[l]=0;
				path2=path2+abs(pos-l);    //computing the path length
				pos=l;
			}
		}
		end2=pos+1;
		if(path1<path2)   //comparing the two paths
		{
			path=path1;
			end=end1;
			pathway=1;
		}
		else
		{
		    path=path2;
			end=end2;
			pathway=2;
		}
	}
	public: void transition(int input)
	{
		int l,m,pos;
		pos=state;
		if(input==1)
		{
			for(l=pos-1;l<=max-1;l++)
			{
				if((stop[l]==1) or (stop[l]==2))
				{
				    stop[l]=0;
					state=l+1;
					break;
				}
			}
			cout<<"Moving up from floor "<<pos<<" to floor "<<state<<"\n";
			
		}
		else
		{
			for(l=pos-1;l>=min-1;l--)
			{
				if((stop[l]==-1) or (stop[l]==2))
				{
				    stop[l]=0;
					state=l+1;
					break;
				}
			}
			cout<<"Moving down from floor "<<pos<<" to floor "<<state<<"\n";
		}
	}

};
int main()
{
	int i,j,k;
	FSM lift;
	lift.start();
	lift.compute_path();
	if(lift.pathway==1)                                      //if path 1 is shorter
	{
		while((lift.state!=lift.min) and (lift.state!=lift.end))  //going down till minimum floor is not reached
		{
			lift.transition(-1);                              //shows the transition
		}
		while((lift.state!=lift.max) and (lift.state!=lift.end))  //going up till maximum floor is not reached
		{
			lift.transition(1);
		}
		while(lift.state!=lift.end)                               //going down till end floor is not reached
		{
			lift.transition(-1);
		}
	}
	else							//if path 2 is shorter
	{
		while((lift.state!=lift.max) and (lift.state!=lift.end))  //going up till maximum floor is not reached
		{
			lift.transition(1);
		}
		while((lift.state!=lift.min) and (lift.state!=lift.end))  //going down till minimum floor is not reached
		{
			lift.transition(-1);
		}
		while(lift.state!=lift.end)                               //going up till end floor is not reached
		{
			lift.transition(1);
		}
	}
}




