#pragma once

#include "ofMain.h"

class Rect {
public:
	Rect(ofPoint point) {
		this->point = point;
		m_visible = true;
	}
	void setX(int x) {
		point.x = x;
	}
	void setY(int y) {
		point.y = y;
	}
	int getX() const {
		return point.x;
	}
	int getY() const {
		return point.y;
	}
	void setVisible(bool v) {
		m_visible = v;
	}
	bool getVisible() {
		return m_visible;
	}

private:
	ofPoint point;
	bool m_visible;
};  //state 0                  state 1               state 2          state 3
//  0  0   02   0	       0123   01  0     0          0   103      120    0
//  1  21  1   123               23   123  21		   1    2         3    13
//  2   3  3								3         23				   2
//  3	  
class Figure {
public:
	enum Type {                
		type1,
		type2,
		type3,
		type4
	};
	Figure(Type t) {
		m_type = t;
		isEnd = false;
		m_state = 0;
		color = ofColor(ofRandom(255), ofRandom(255), ofRandom(255));
	}

	void setup(vector<vector<bool>*> &rectZone);
	void update(vector<vector<bool>*> &rectZone);
	void moveDown(int maxHeight);
	void draw();
	void setEnd(bool end) {
		isEnd = end;
	}
	bool getEnd() const {
		return isEnd;
	}
	void move(bool plus, vector<vector<bool>*> &rectZone);
	vector<Rect*> getRects() {
		return rects;
	}
	Type getType() const {
		return m_type;
	}
	void setState(int state) {
		if (state > 3) {
			state = 0;
		}
		else if (state < 0) {
			state = 3;
		}
		m_state = state;
	}
	bool checkInteract(vector<vector<bool>*> &rectZone);
	bool checkOutFromRectangle();
	void updateState(bool plus, vector<vector<bool>*> &rectZone);
	void updateCoords();
	void setFigureRectZone(bool set, vector<vector<bool>*> &rectZone);
	int getState() const {
		return m_state;
	}
	void setLeftTopPoint(ofPoint point);
	ofPoint getltPoint() const {
		return leftTopPoint;
	}
	bool checkTransformFigure(bool plus, vector<vector<bool>*> &rectZone);
	ofColor getColor() const {
		return color;
	}

private:
	Type m_type;
	int m_state;
	vector<Rect*> rects;
	ofPoint leftTopPoint;
	ofColor color;
	bool isEnd;
};

class ofApp : public ofBaseApp{
	public:
		enum GameState {
			Play,
			End
		};
		void setup();
		void update();
		void draw();
		void recountRectZone();
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y);
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		vector<Figure*> figures;
		vector<vector<bool>*> *rectZone; //x = 8  widthGameRect / 50    y = 15 ofGetHeigth() / 50 
		int timer;
		GameState state;
};
