/*	Created by Steve Varga on 8/22/12.
 *	Copyright 2012 __MyCompanyName__. All rights reserved.
 */

#include "poObject.h"
#include "poThreadCenter.h"

class poThreadCenterExampleApp : public poObject {
public:
	poThreadCenterExampleApp();
	virtual ~poThreadCenterExampleApp();
	
    virtual void update();
    
    virtual void draw();
	
    virtual void eventHandler(poEvent *event);
	
    virtual void messageHandler(const std::string &msg, const poDictionary& dict=poDictionary());
    
    poThreadCenterWorker *simpleWorker;
};

