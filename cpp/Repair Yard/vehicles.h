using namespace std;

class vehicle{
public:
	vehicle(){
		type = 0;
		next = NULL;
	};

	void display(){
		if (type == 1)
		{
			cout << "Damaged Tank One with broken parts: ";
			for (list<int>::iterator it = partsList.begin(); it != partsList.end(); it++) // Display individual parts until end of list
			{
				cout << *it << " ";
			}
			cout << "\n";
		}
		else if (type == 2)
		{
			cout << "Damaged Tank Two with broken parts: ";
			for (list<int>::iterator it = partsList.begin(); it != partsList.end(); it++)
			{
				cout << *it << " ";
			}
			cout << "\n";
		}
		else if (type == 3)
		{
			cout << "Damaged Tank Three with broken parts: ";
			for (list<int>::iterator it = partsList.begin(); it != partsList.end(); it++)
			{
				cout << *it << " ";
			}
			cout << "\n";
		}
		else if (type == 4)
		{
			cout << "Damaged Wheeled Vehicle One with broken parts: ";
			for (list<int>::iterator it = partsList.begin(); it != partsList.end(); it++)
			{
				cout << *it << " ";
			}
			cout << "\n";
		}
		else if (type == 5)
		{
			cout << "Damaged Wheeled Vehicle Two with broken parts: ";
			for (list<int>::iterator it = partsList.begin(); it != partsList.end(); it++)
			{
				cout << *it << " ";
			}
			cout << "\n";
		}
		else if (type == 6)
		{
			cout << "Damaged Half-Track with broken parts: ";
			for (list<int>::iterator it = partsList.begin(); it != partsList.end(); it++)
			{
				cout << *it << " ";
			}
			cout << "\n";
		}
	}

	void addPart(int a)
	{
		partsList.push_back(a);
	}

	void setNext(vehicle* n){
		next = n;
	}

	vehicle* getNext(){
		return next;
	}

	int numberOfParts(){
		int x = 0;
		for (list<int>::iterator it = partsList.begin(); it != partsList.end(); it++)
		{
			x++;
		}
		return x;
	}

	int getPart(int x){
		list<int>::iterator it = partsList.begin();
		advance(it,x);
		return *it;
	}

	void removePart(int x){
		list<int>::iterator it = partsList.begin();
		advance(it,x);
		partsList.erase(it);
	}

	int getType(){
		return type;
	}

protected:
	int type;

private:
	vehicle* next;
	list<int> partsList;
};

//---------------------------------------------------------

class tankOne:public vehicle{
public:
	tankOne(){
		type = 1;
	};
};

//---------------------------------------------------------

class tankTwo:public vehicle{
public:
	tankTwo(){
		type = 2;
	};
};

//---------------------------------------------------------

class tankThree:public vehicle{
public:
	tankThree(){
		type = 3;
	};
};

//---------------------------------------------------------

class wheelOne:public vehicle{
public:
	wheelOne(){
		type = 4;
	};
};

//---------------------------------------------------------

class wheelTwo:public vehicle{
public:
	wheelTwo(){
		type = 5;
	};
};

//---------------------------------------------------------

class halfTrack:public vehicle{
public:
	halfTrack(){
		type = 6;
	};
};
