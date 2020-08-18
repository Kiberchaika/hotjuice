#pragma once

#include "MurkaTypes.h"
#include "MurkaContext.h"
#include "MurkaView.h"
#include "MurkaInputEventsRegister.h"
#include "MurkaAssets.h"
#include "MurkaLinearLayoutGenerator.h"
#include "MurkaSurface.h"

using namespace murka;

class TestSurface : public MurkaViewInterface<TestSurface>, public MurkaSurface {
public:
    MURKA_VIEW_DRAW_FUNCTION  {

        auto params = (Parameters*)parametersObject;
        auto results = (Results*)resultObject;
        bool inside = context.isHovered() * !areInteractiveChildrenHovered(context) * hasMouseFocus(context);

        surface.scrollwheelShouldZoom = false;
        surface.scrollwheelShouldPan = true;
        surface.clickShouldStartPanning = inside;
        
        // if you want to limit pan, panLimits should be ALL SET AT ONCE
        panLimits.position.x = 0;
        panLimits.position.y = 0;
        panLimits.size.y = (100) * 30 - context.getSize().y;
        panLimits.size.x = 0;
        
        // Then use .panOffset to transform view
        
        int howManyLabelsDidWeDraw = 0;
        for (int i = 0; i < 100; i++) {
            
            MurkaShape labelShape = {0, 30 * i - panOffset.y,
                200, 30};
            
            MurkaShape frameRectangle = {0, 0, context.getSize().x, context.getSize().y};
            
            if (labelShape.intersectsWithRectangle(frameRectangle)) {
                drawWidget<Label>(context, {"Label #" + std::to_string(i)},
                                  labelShape);
                 
                howManyLabelsDidWeDraw++;
            }
        }
       
        context.renderer->setColor(0);
		context.renderer->drawRectangle(0, 0, 200, 30);
        drawWidget<Label>(context, {"Labels drawn: " + std::to_string(howManyLabelsDidWeDraw)}, {0, 0, 200, 30});
        
        drawWidget<ScrollBar>(context, {this}, {context.getSize().x - 27, 2, 20, context.getSize().y - 4});
        
        std::function<void(MurkaPoint)> doubleClickCallback = [](MurkaPoint p){};

        surfaceUpdate(context);
    };

    struct Parameters {
        float somevalue = 0.0;

        Parameters() {}
        Parameters(float someval) { somevalue = someval; } // a convenience initializer
    };

    typedef bool Results;

};

