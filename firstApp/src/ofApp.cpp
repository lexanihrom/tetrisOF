#include "ofApp.h"
#include <iostream>
//--------------------------------------------------------------
void ofApp::setup(){
	ofFill();
	ofSetColor(0, 0, 0);		
	ofRect(ofPoint(100, 0), 400, ofGetHeight());
	state = Play;
	rectZone = new vector<vector<bool>*>;
	for (int i = 0; i < ofGetHeight() / 50; i++) {     // 750 / 50
		vector<bool> *v = new vector<bool>;
		for (int j = 0; j < 8; j++) { // 400 / 50
			v->push_back(false); //заполняем все 0;
		}
		rectZone->push_back(v);
	}
	Figure *f = new Figure((Figure::Type)((int)ofRandom(4)));
	f->setup(*rectZone);
	figures.push_back(f);
	ofSetFrameRate(60);
}

//--------------------------------------------------------------
void ofApp::update(){
	if (state == Play) {  //состояние игры
		if (timer / 15) {    //пересчет раз в 15 кадров
			vector<int> removeLines;    
			for (int i = 0; i < rectZone->size(); i++) {
				bool removeLine = true;
				for (int j = 0; j < rectZone->at(i)->size(); j++) {   //проверка на заполненную линию
					if (rectZone->at(i)->at(j) == false) {
						removeLine = false;
						break;
					}
				}
				if (removeLine) {
					removeLines.push_back(i);
				}
			}
			if (!removeLines.empty()) {
				for (int i = 0; i < removeLines.size(); i++) {
					int maxHeight = removeLines.at(i) * 50;
					for (int j = 0; j < figures.size(); j++) {
						Figure *f = figures.at(j);
						f->moveDown(maxHeight);   //если заполнена двигаем все фигуры сверху
					}
				}
				recountRectZone();   //пересчитываем поле
			}

			bool reset = true;
			for (int i = 0; i < figures.size(); i++) {
				Figure *f = figures.at(i);
				if (!f->getEnd()) {   //если есть текущая активная то обновляем ее
					reset = false;
					f->update(*rectZone);
				}
			}
			if (reset) {   //текущей нет, создаем новую
				Figure *f = new Figure((Figure::Type)((int)ofRandom(4)));
				f->setup(*rectZone);
				figures.push_back(f);
				if (f->checkInteract(*rectZone)) {  //если столкновение, конец игры
					state = GameState::End;
				}
			}
			timer = 0;  //обновление таймера
		}
		else {
			++timer;
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofSetColor(0, 0, 0);
	ofRect(ofPoint(100, 0), 400, ofGetHeight());   //отрисовка поля
	for (int i = 0; i < figures.size(); i++) {
		Figure *f = figures.at(i);    //отрисовка фигур
		f->draw();
	}   
	/*ofSetColor(255, 255, 255);
	for (int i = 0; i < rectZone->size(); i++) {
		for (int j = 0; j < rectZone->at(i)->size(); j++) {
			if (rectZone->at(i)->at(j)) {
				ofDrawBitmapString("T", j * 50 + 100, i * 50 + 10);   проверка контейнера с фигурами
			}
			else {
				ofDrawBitmapString("F", j * 50 + 100, i * 50 + 10);
			}			
		}
	}*/
	if (state == End) {
		ofSetColor(0, 0, 0);
		ofDrawBitmapString("End game", 0, 100);   //надпись конца игры
	}
}

void ofApp::recountRectZone()
{
	for (int i = 0; i < rectZone->size(); i++) {
		for (int j = 0; j < rectZone->at(i)->size(); j++) {    //заполняем контейнер 0
			rectZone->at(i)->at(j) = false;
		}
	}
	for (int i = 0; i < figures.size(); i++) {
		Figure *f = figures.at(i);
		for (int j = 0; j < f->getRects().size(); j++) {
			Rect *r = f->getRects().at(j);
			if (r->getVisible()) {
				rectZone->at(r->getY() / 50)->at((r->getX() - 100) / 50) = true;    //обновляем значения
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (state == Play) {
		if (key == OF_KEY_LEFT) {
			for (int i = 0; i < figures.size(); i++) {
				Figure *f = figures.at(i);
				if (!f->getEnd()) {    //если текущая то двигаем
					f->move(false, *rectZone);
				}
			}
		}
		else if (key == OF_KEY_RIGHT) {
			for (int i = 0; i < figures.size(); i++) {
				Figure *f = figures.at(i);
				if (!f->getEnd()) { //если текущая то двигаем
					f->move(true, *rectZone);
				}
			}
		}
		else if (key == OF_KEY_UP) {
			for (int i = 0; i < figures.size(); i++) {
				Figure *f = figures.at(i); //если текущая то проверяем на возможность поворота и поворачиваем
				if (!f->getEnd() && f->checkTransformFigure(true, *rectZone)) { 
					f->updateState(true, *rectZone);
				}
			}
		}
		else if (key == OF_KEY_DOWN) {
			for (int i = 0; i < figures.size(); i++) {
				Figure *f = figures.at(i); //если текущая то проверяем на возможность поворота и поворачиваем
				if (!f->getEnd() && f->checkTransformFigure(false, *rectZone)) {
					f->updateState(false, *rectZone);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

void Figure::setup(vector<vector<bool>*> &rectZone)
{
	Rect *r0;
	Rect *r1;
	Rect *r2;
	Rect *r3;   //создание фигур по типу
	if (m_type == Figure::type1) {
		r3 = new Rect(ofPoint(ofGetWidth() / 2, 150));
		r1 = new Rect(ofPoint(ofGetWidth() / 2, 50));
		r2 = new Rect(ofPoint(ofGetWidth() / 2, 100));
		r0 = new Rect(ofPoint(ofGetWidth() / 2, 0));
	}
	else if (m_type == Figure::type2) {
		r3 = new Rect(ofPoint(ofGetWidth() / 2 + 50, 100));
		r0 = new Rect(ofPoint(ofGetWidth() / 2, 0));
		r2 = new Rect(ofPoint(ofGetWidth() / 2, 50));
		r1 = new Rect(ofPoint(ofGetWidth() / 2 + 50, 50));
	}
	else if (m_type == Figure::type3){
		r3 = new Rect(ofPoint(ofGetWidth() / 2, 100));
		r0 = new Rect(ofPoint(ofGetWidth() / 2, 0));
		r1 = new Rect(ofPoint(ofGetWidth() / 2, 50));
		r2 = new Rect(ofPoint(ofGetWidth() / 2 + 50, 0));
	}
	else {
		r1 = new Rect(ofPoint(ofGetWidth() / 2 - 50, 50));
		r0 = new Rect(ofPoint(ofGetWidth() / 2, 0));
		r2 = new Rect(ofPoint(ofGetWidth() / 2, 50));
		r3 = new Rect(ofPoint(ofGetWidth() / 2 + 50, 50));
	}
	rects.push_back(r0);
	rects.push_back(r1);
	rects.push_back(r2);  //заполняем массив
	rects.push_back(r3);
	setLeftTopPoint(ofPoint(ofGetWidth() / 2, 0));  //задаем точку 0 блока(от него все расчеты идут)
}

void Figure::update(vector<vector<bool>*> &rectZone)
{
	if (!getEnd()) {   //если текущая то проверяем за выход снизу и есть ли снизу столкновения
		if (!checkOutFromRectangle() && !checkInteract(rectZone)) {
			setLeftTopPoint(ofPoint(leftTopPoint.x, leftTopPoint.y + 50));
		}
		else {
			//останавливаем и заполняем контейнер поля фигурой
			setFigureRectZone(true, rectZone);
			setEnd(true);
		}
	}
}

bool Figure::checkInteract(vector<vector<bool>*> &rectZone)
{
	//проверка каждого поля и состояния на столкновения снизу(проверка по карте в .h) 
	bool checkInteract = false;
	if (getType() == type1 && (getState() == 0 || getState() == 2)) {
		if (rects.at(3)->getVisible() && rectZone.at((rects.at(3)->getY() + 50) / 50)->at((rects.at(3)->getX() - 100) / 50)) {
			checkInteract = true;
		}
	}
	else if (getType() == type1 && (getState() == 1 || getState() == 3)) {
		for (int i = 0; i < rects.size(); i++) {
			Rect *r = rects.at(i);
			if (r->getVisible() && rectZone.at((r->getY() + 50) / 50 )->at((r->getX() - 100) / 50)) {
				checkInteract = true;
				break;
			}
		}
	}
	else if (((getType() == type2 || getType() == type3) && (getState() == 0 || getState() == 2)) ||
		(getType() == type4 && (getState() == 1 || getState() == 3))) {
		if (rects.at(3)->getVisible() &&  rectZone.at((rects.at(3)->getY() + 50) / 50 )->at((rects.at(3)->getX() - 100) / 50)) {
			checkInteract = true;
		}
		else if (rects.at(2)->getVisible() &&  rectZone.at((rects.at(2)->getY() + 50) / 50)->at((rects.at(2)->getX() - 100) / 50)) {
			checkInteract = true;
		}
	}
	else if (((getType() == type2 || getType() == type3) && (getState() == 1 || getState() == 3)) ||
		(getType() == type4 && (getState() == 0 || getState() == 2)))
	{
		for (int i = 1; i < rects.size(); i++) {
			Rect *r = rects.at(i);
			if (r->getVisible() && rectZone.at((r->getY() + 50) / 50)->at((r->getX() - 100) / 50)) {
				checkInteract = true;
				break;
			}
		}
	}
	return checkInteract;
}

bool Figure::checkOutFromRectangle()
{
	//проверка за выход снизу экрана
	for (int i = 0; i < rects.size(); i++) {
		if (rects.at(i)->getY() >= 700) {
			return true;
		}
	}
	return false;
}

void Figure::updateState(bool plus, vector<vector<bool>*> &rectZone)
{
	//обновление состояния, пересчет координат и отрисовка
	int newState = plus ? getState() + 1 : getState() - 1;
	setState(newState);
	updateCoords();
	draw();
}

void Figure::updateCoords()
{
	//пересчет координат относительно 0 блока
	rects.at(0)->setX(leftTopPoint.x);
	rects.at(0)->setY(leftTopPoint.y);
	if (getType() == type1 && (getState() == 0 || getState() == 2)) {
		rects.at(1)->setX(leftTopPoint.x);
		rects.at(1)->setY(leftTopPoint.y + 50);
		rects.at(2)->setX(leftTopPoint.x);
		rects.at(2)->setY(leftTopPoint.y + 100);
		rects.at(3)->setX(leftTopPoint.x);
		rects.at(3)->setY(leftTopPoint.y + 150);
	} 
	else if (getType() == type1 && (getState() == 1 || getState() == 3)) {
		rects.at(1)->setX(leftTopPoint.x + 50);
		rects.at(1)->setY(leftTopPoint.y);
		rects.at(2)->setX(leftTopPoint.x + 100);
		rects.at(2)->setY(leftTopPoint.y);
		rects.at(3)->setX(leftTopPoint.x + 150);
		rects.at(3)->setY(leftTopPoint.y);
	}
	else if (getType() == type2 && (getState() == 0 || getState() == 2)) {
		rects.at(1)->setX(leftTopPoint.x + 50);
		rects.at(1)->setY(leftTopPoint.y + 50);
		rects.at(2)->setX(leftTopPoint.x);
		rects.at(2)->setY(leftTopPoint.y + 50);
		rects.at(3)->setX(leftTopPoint.x + 50);
		rects.at(3)->setY(leftTopPoint.y + 100);
	}
	else if (getType() == type2 && (getState() == 1 || getState() == 3)) {
		rects.at(1)->setX(leftTopPoint.x + 50);
		rects.at(1)->setY(leftTopPoint.y);
		rects.at(2)->setX(leftTopPoint.x - 50);
		rects.at(2)->setY(leftTopPoint.y + 50);
		rects.at(3)->setX(leftTopPoint.x);
		rects.at(3)->setY(leftTopPoint.y + 50);
	}
	else if (getType() == type3 && getState() == 0) {
		rects.at(1)->setX(leftTopPoint.x);
		rects.at(1)->setY(leftTopPoint.y + 50);
		rects.at(2)->setX(leftTopPoint.x + 50);
		rects.at(2)->setY(leftTopPoint.y);
		rects.at(3)->setX(leftTopPoint.x);
		rects.at(3)->setY(leftTopPoint.y + 100);
	}
	else if (getType() == type3 && getState() == 1) {
		rects.at(1)->setX(leftTopPoint.x);
		rects.at(1)->setY(leftTopPoint.y + 50);
		rects.at(2)->setX(leftTopPoint.x + 50);
		rects.at(2)->setY(leftTopPoint.y + 50);
		rects.at(3)->setX(leftTopPoint.x + 100);
		rects.at(3)->setY(leftTopPoint.y + 50);
	}
	else if (getType() == type3 && getState() == 2) {
		rects.at(1)->setX(leftTopPoint.x);
		rects.at(1)->setY(leftTopPoint.y + 50);
		rects.at(2)->setX(leftTopPoint.x - 50);
		rects.at(2)->setY(leftTopPoint.y + 100);
		rects.at(3)->setX(leftTopPoint.x);
		rects.at(3)->setY(leftTopPoint.y + 100);
	}
	else if (getType() == type3 && getState() == 3) {
		rects.at(1)->setX(leftTopPoint.x - 100);
		rects.at(1)->setY(leftTopPoint.y);
		rects.at(2)->setX(leftTopPoint.x - 50);
		rects.at(2)->setY(leftTopPoint.y);
		rects.at(3)->setX(leftTopPoint.x);
		rects.at(3)->setY(leftTopPoint.y + 50);
	}
	else if (getType() == type4 && getState() == 0) {
		rects.at(1)->setX(leftTopPoint.x - 50);
		rects.at(1)->setY(leftTopPoint.y + 50);
		rects.at(2)->setX(leftTopPoint.x);
		rects.at(2)->setY(leftTopPoint.y + 50);
		rects.at(3)->setX(leftTopPoint.x + 50);
		rects.at(3)->setY(leftTopPoint.y + 50);
	}
	else if (getType() == type4 && getState() == 1) {
		rects.at(1)->setX(leftTopPoint.x);
		rects.at(1)->setY(leftTopPoint.y + 50);
		rects.at(2)->setX(leftTopPoint.x - 50);
		rects.at(2)->setY(leftTopPoint.y + 50);
		rects.at(3)->setX(leftTopPoint.x);
		rects.at(3)->setY(leftTopPoint.y + 100);
	}
	else if (getType() == type4 && getState() == 2) {
		rects.at(1)->setX(leftTopPoint.x - 50);
		rects.at(1)->setY(leftTopPoint.y);
		rects.at(2)->setX(leftTopPoint.x);
		rects.at(2)->setY(leftTopPoint.y + 50);
		rects.at(3)->setX(leftTopPoint.x + 50);
		rects.at(3)->setY(leftTopPoint.y);
	}
	else if (getType() == type4 && getState() == 3) {
		rects.at(1)->setX(leftTopPoint.x);
		rects.at(1)->setY(leftTopPoint.y + 50);
		rects.at(2)->setX(leftTopPoint.x);
		rects.at(2)->setY(leftTopPoint.y + 100);
		rects.at(3)->setX(leftTopPoint.x + 50);
		rects.at(3)->setY(leftTopPoint.y + 50);
	}
}

void Figure::setFigureRectZone(bool set, vector<vector<bool>*>& rectZone)
{
	//заполнение поля фигурой
	for (int i = 0; i < rects.size(); i++) {
		rectZone.at(rects.at(i)->getY() / 50)->at((rects.at(i)->getX() - 100) / 50) = set;
	}
}

void Figure::setLeftTopPoint(ofPoint point)
{
	//изменение координаты 0 блока и пересчет всех остальных координат
	leftTopPoint = point;
	updateCoords();
}

bool Figure::checkTransformFigure(bool plus, vector<vector<bool>*> &rectZone)
{
	//проверка каждого типа и состояния на возможность поворота
	bool checkUpdate = false;
	int newState = plus ? getState() + 1 > 3 ? 0 : getState() + 1 : getState() - 1 < 0 ? 3 : getState() - 1;
	if (getType() == type1 && (newState == 1 || newState == 3)) {
		if (leftTopPoint.x + 150 < 500 && leftTopPoint.y < 700 &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100 + 50) / 50) &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100 + 100) / 50) &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100 + 150) / 50)) {
			checkUpdate = true;
		}
	}
	else if (getType() == type1 && (newState == 0 || newState == 2)) {
		if (leftTopPoint.y + 150 < 700 && 
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 100) / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 150) / 50)->at((leftTopPoint.x - 100) / 50)) {
			checkUpdate = true;
		}
	}
	else if (getType() == type2 && (newState == 1 || newState == 3)) {
		if (leftTopPoint.x - 50 >= 100 && leftTopPoint.y + 50 < 700 &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100 + 50) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100 - 50) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100) / 50)) {
			checkUpdate = true;
		}
	}
	else if (getType() == type2 && (newState == 0 || newState == 2)) {
		if (leftTopPoint.y + 100 < 700 && 
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100 + 50) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 100) / 50)->at((leftTopPoint.x - 100 + 50) / 50)) {
			checkUpdate = true;
		}
	} 
	else if (getType() == type3 && newState == 1) {
		if (leftTopPoint.x + 100 < 500 && leftTopPoint.y + 50 < 700 &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100 + 50) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100 + 100) / 50)) {
			checkUpdate = true;
		}
	}
	else if (getType() == type3 && newState == 0) {
		if (leftTopPoint.x + 50 < 500 && leftTopPoint.y + 100 < 700 &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y) / 50)->at((leftTopPoint.x - 100 + 50) / 50) &&
			!rectZone.at((leftTopPoint.y + 100) / 50)->at((leftTopPoint.x - 100) / 50)) {
			checkUpdate = true;
		}
	}
	else if (getType() == type3 && newState == 3) {
		if (leftTopPoint.x - 100 >= 100 && leftTopPoint.y + 50 < 700 &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y) / 50)->at((leftTopPoint.x - 100 - 100) / 50) &&
			!rectZone.at((leftTopPoint.y) / 50)->at((leftTopPoint.x - 100 - 50) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100) / 50)) {
			checkUpdate = true;
		}
	}
	else if (getType() == type3 && newState == 2) {
		if (leftTopPoint.x - 50 >= 100 && leftTopPoint.y + 100 < 700 &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 100) / 50)->at((leftTopPoint.x - 100 - 50) / 50) &&
			!rectZone.at((leftTopPoint.y + 100) / 50)->at((leftTopPoint.x - 100) / 50)) {
			checkUpdate = true;
		}
	}
	else if (getType() == type4 && newState == 1) {
		if (leftTopPoint.x - 50 >= 100 && leftTopPoint.y + 100 < 700 &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100 - 50) / 50) &&
			!rectZone.at((leftTopPoint.y + 100) / 50)->at((leftTopPoint.x - 100) / 50)) {
			checkUpdate = true;
		}
	}
	else if (getType() == type4 && newState == 0) {
		if (leftTopPoint.x - 50 >= 100 && leftTopPoint.x + 50 < 500 && leftTopPoint.y + 50 < 700 &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100 - 50) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100 + 50) / 50)) {
			checkUpdate = true;
		}
	}
	else if (getType() == type4 && newState == 3) {
		if (leftTopPoint.x + 50 < 500 && leftTopPoint.y + 100 < 700 &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 100) / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100 + 50) / 50)) {
			checkUpdate = true;
		}
	}
	else if (getType() == type4 && newState == 2) {
		if (leftTopPoint.x - 50 >= 100 && leftTopPoint.x + 50 < 500 && leftTopPoint.y + 50 < 700 &&
			!rectZone.at(leftTopPoint.y / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y) / 50)->at((leftTopPoint.x - 100 - 50) / 50) &&
			!rectZone.at((leftTopPoint.y + 50) / 50)->at((leftTopPoint.x - 100) / 50) &&
			!rectZone.at((leftTopPoint.y) / 50)->at((leftTopPoint.x - 100 + 50) / 50)) {
			checkUpdate = true;
		}
	}
	return checkUpdate;
}

void Figure::moveDown(int maxHeight)
{
	//передвигаем вниз после исчезания строки, которые на строке скрываем
	if (getEnd()) {
		for (int i = 0; i < rects.size(); i++) {
			Rect *r = rects.at(i);
			if (r->getY() == maxHeight) {
				r->setVisible(false);
			}
			if (r->getY() < maxHeight) {
				r->setY(r->getY() + 50);
			}
		}
	}
}

void Figure::draw()
{
	//если видимость тру то отрисовывем
	for (int i = 0; i < rects.size(); i++) {
		ofSetColor(getColor());
		Rect *r = rects.at(i);
		if (r->getVisible()) {
			ofRect(ofPoint(r->getX(), r->getY()), 50, 50);
		}
	}
}

void Figure::move(bool m, vector<vector<bool>*> &rectZone)
{
	//двигаем влево или вправо, если возможно
	bool move = true;
	int m2 = m ? 50 : -50;
	for (int i = 0; i < rects.size(); i++) {
		Rect *r = rects.at(i);
		if (r->getX() + m2 >= 500 || r->getX() + m2 < 100 ||
			rectZone.at(r->getY() / 50)->at((r->getX() + m2 - 100) / 50)) {
			move = false;
			break;
		}
	}
	if (move) {
		setLeftTopPoint(ofPoint(leftTopPoint.x + m2, leftTopPoint.y));
	}
}
