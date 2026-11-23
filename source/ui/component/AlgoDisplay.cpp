#include "AlgoDisplay.h"
#include "../DXLookNFeel.h"

void AlgoDisplay::displayOp(juce::Graphics& g, char id, int x, int y, char link, char fb) {
    const float LINE_SZ = 2.0f;

    const int opNumber = static_cast<int>(id);
    const juce::String t = juce::String(opNumber);

    const int opIndex = opNumber - 1;
    const bool opOn = (opIndex >= 0 && opIndex < 6) ? static_cast<bool>(opSwitch[opIndex]) : true;

    const int px = x * 25 + 3;
    const int py = y * 21 + 5;


    juce::Rectangle<float> opBox(static_cast<float>(px), static_cast<float>(py), 16.0f, 13.0f);

    if (opOn) {
        g.setColour(DXLookNFeel::lightBackground);
        g.fillRoundedRectangle(opBox, 2.0f);

        g.setColour(DXLookNFeel::fillColour);
        g.drawRoundedRectangle(opBox, 2.0f, 1.2f);

        g.setColour(DXLookNFeel::textColour);
    } else {
        //bypassed
        //g.setColour(DXLookNFeel::borderOutline);
        //g.fillRoundedRectangle(opBox, 2.0f);

        g.setColour(DXLookNFeel::borderOutline);
        g.drawRoundedRectangle(opBox, 2.0f, 1.0f);

        g.setColour(DXLookNFeel::secondaryText);
    }

    g.setFont(juce::Font(juce::FontOptions(10.0f).withStyle("Bold")));
    g.drawText(t, opBox.toNearestInt(), juce::Justification::centred, false);

    g.setColour(opOn ? DXLookNFeel::fillColour : DXLookNFeel::borderOutline);

    switch (link) {
    case 0: 
        g.drawLine(px + 8.0f, py + 13.0f, px + 8.0f, py + 21.0f, LINE_SZ);
        break;

    case 1:  
        g.drawLine(px + 8.0f, py + 13.0f, px + 8.0f, py + 18.0f, LINE_SZ);
        g.drawLine(px + 7.0f, py + 18.0f, px + 34.0f, py + 18.0f, LINE_SZ);
        break;

    case 2:  
        g.drawLine(px + 8.0f, py + 13.0f, px + 8.0f, py + 19.0f, LINE_SZ);
        break;

    case 3:  
        g.drawLine(px + 8.0f, py + 13.0f, px + 8.0f, py + 21.0f, LINE_SZ);
        g.drawLine(px + 7.0f, py + 18.0f, px + 34.0f, py + 18.0f, LINE_SZ);
        g.drawLine(px + 34.0f, py + 17.0f, px + 34.0f, py + 21.0f, LINE_SZ);
        break;

    case 4:  
        g.drawLine(px + 8.0f, py + 13.0f, px + 8.0f, py + 21.0f, LINE_SZ);
        g.drawLine(px + 7.0f, py + 18.0f, px + 34.0f, py + 18.0f, LINE_SZ);
        g.drawLine(px + 34.0f, py + 17.0f, px + 34.0f, py + 21.0f, LINE_SZ);
        g.drawLine(px - 17.0f, py + 18.0f, px + 8.0f, py + 18.0f, LINE_SZ);
        g.drawLine(px - 17.0f, py + 17.0f, px - 17.0f, py + 21.0f, LINE_SZ);
        break;

    case 6:
        g.drawLine(px + 8.0f, py + 13.0f, px + 8.0f, py + 18.0f, LINE_SZ);
        g.drawLine(px + 7.0f, py + 18.0f, px + 58.0f, py + 18.0f, LINE_SZ);
        break;

    case 7:
        g.drawLine(px + 8.0f, py + 13.0f, px + 8.0f, py + 19.0f, LINE_SZ);
        g.drawLine(px - 17.0f, py + 18.0f, px + 9.0f, py + 18.0f, LINE_SZ);
        break;

    default:
        break;
    }

    // feedback
    if (fb != 0) {
        g.setColour(opOn ? DXLookNFeel::fillColour : DXLookNFeel::borderOutline);

        switch (fb) {
        case 1:
            g.drawLine(px + 7.0f, py, px + 8.0f, py - 5.0f, LINE_SZ);
            g.drawLine(px + 8.0f, py - 4.0f, px + 21.0f, py - 4.0f, LINE_SZ);
            g.drawLine(px + 20.0f, py - 4.0f, px + 20.0f, py + 15.0f, LINE_SZ);
            g.drawLine(px + 19.0f, py + 15.0f, px + 20.0f, py + 16.0f, LINE_SZ);
            g.drawLine(px + 8.0f, py + 15.0f, px + 20.0f, py + 15.0f, LINE_SZ);
            break;

        case 2: 
            g.drawLine(px + 7.0f, py, px + 8.0f, py - 5.0f, LINE_SZ);
            g.drawLine(px + 8.0f, py - 4.0f, px + 20.0f, py - 4.0f, LINE_SZ);
            g.drawLine(px + 19.0f, py - 4.0f, px + 19.0f, py + 59.0f, LINE_SZ);
            g.drawLine(px + 8.0f, py + 58.0f, px + 19.0f, py + 58.0f, LINE_SZ);
            break;

        case 3: 
            g.drawLine(px + 7.0f, py, px + 8.0f, py - 5.0f, LINE_SZ);
            g.drawLine(px + 8.0f, py - 4.0f, px + 20.0f, py - 4.0f, LINE_SZ);
            g.drawLine(px + 19.0f, py - 4.0f, px + 19.0f, py + 37.0f, LINE_SZ);
            g.drawLine(px + 8.0f, py + 36.0f, px + 19.0f, py + 36.0f, LINE_SZ);
            break;

        case 4:  
            g.drawLine(px + 7.0f, py, px + 8.0f, py - 5.0f, LINE_SZ);
            g.drawLine(px + 8.0f, py - 4.0f, px - 4.0f, py - 4.0f, LINE_SZ);
            g.drawLine(px - 3.0f, py - 4.0f, px - 3.0f, py + 15.0f, LINE_SZ);
            g.drawLine(px - 3.0f, py + 15.0f, px + 8.0f, py + 15.0f, LINE_SZ);
            g.drawLine(px + 8.0f, py + 15.0f, px + 8.0f, py + 12.0f, LINE_SZ);
            break;

        default:
            break;
        }
    }
}


void AlgoDisplay::paint(juce::Graphics& g) {
    const float w = static_cast<float>(getWidth());
    const float h = static_cast<float>(getHeight());
    g.setColour(DXLookNFeel::background);
    g.fillRoundedRectangle(getLocalBounds().toFloat(), 3.0f);

    // Border
    //g.setColour(DXLookNFeel::borderOutline);
    //g.drawRoundedRectangle(getLocalBounds().toFloat().reduced(0.5f), 3.0f, 1.0f);


    const float totalGridW = 150.0f;
    const float totalGridH = 84.0f;
    const float originX = std::floor((w - totalGridW) * 0.5f);
    const float originY = std::floor((h - totalGridH) * 0.5f);
    juce::Rectangle<float> algoTag(originX, originY - 18.0f, 26.0f, 15.0f);

    //or just in corner
    //juce::Rectangle<float> algoTag(6.0f, 6.0f, 26.0f, 16.0f);

    g.setColour(DXLookNFeel::lightBackground);
    g.fillRoundedRectangle(algoTag, 3.0f);

    g.setColour(DXLookNFeel::fillColour);
    g.drawRoundedRectangle(algoTag, 3.0f, 1.2f);

    int algo = algoValue;  // 1 to 32
    g.setColour(DXLookNFeel::textColour);
    g.setFont(juce::Font(juce::FontOptions(11.0f).withStyle("Bold")));
    g.drawText(juce::String(algo), algoTag.toNearestInt(), juce::Justification::centred, false);

    
    
    const float offsetX = std::floor((w - totalGridW) * 0.5f);
    const float offsetY = std::floor((h - totalGridH) * 0.5f);

    juce::Graphics::ScopedSaveState state(g);
    g.addTransform(juce::AffineTransform::translation(offsetX, offsetY));

     // placeholder boxes
    //g.setColour(DXLookNFeel::borderOutline);
    //for (int col = 0; col < 6; ++col) {
    //    for (int row = 0; row < 4; ++row) {
    //        int px = col * 25 + 3;
    //        int py = row * 21 + 5;
    //        g.drawRoundedRectangle(static_cast<float>(px), static_cast<float>(py), 16.0f, 13.0f,
    //                               2.0f, 0.8f);
    //    }
    //}

    switch (algo - 1) {
    case 0:
        displayOp(g, 6, 3, 0, 0, 1);
        displayOp(g, 5, 3, 1, 0, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 2, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 1:
        displayOp(g, 6, 3, 0, 0, 0);
        displayOp(g, 5, 3, 1, 0, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 2, 0);
        displayOp(g, 2, 2, 2, 0, 1);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 2:
        displayOp(g, 6, 3, 1, 0, 1);
        displayOp(g, 5, 3, 2, 0, 0);
        displayOp(g, 4, 3, 3, 2, 0);
        displayOp(g, 3, 2, 1, 0, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 3:
        displayOp(g, 6, 3, 1, 0, 2);
        displayOp(g, 5, 3, 2, 0, 0);
        displayOp(g, 4, 3, 3, 2, 0);
        displayOp(g, 3, 2, 1, 0, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 4:
        displayOp(g, 6, 4, 2, 0, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 1, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 5:
        displayOp(g, 6, 4, 2, 0, 3);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 1, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 6:
        displayOp(g, 6, 4, 1, 0, 1);
        displayOp(g, 5, 4, 2, 7, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 2, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 7:
        displayOp(g, 6, 4, 1, 0, 0);
        displayOp(g, 5, 4, 2, 7, 0);
        displayOp(g, 4, 3, 2, 0, 4);
        displayOp(g, 3, 3, 3, 2, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 8:
        displayOp(g, 6, 4, 1, 0, 0);
        displayOp(g, 5, 4, 2, 7, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 2, 0);
        displayOp(g, 2, 2, 2, 0, 1);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 9:
        displayOp(g, 6, 2, 2, 0, 0);
        displayOp(g, 5, 1, 2, 1, 0);
        displayOp(g, 4, 2, 3, 1, 0);
        displayOp(g, 3, 3, 1, 0, 1);
        displayOp(g, 2, 3, 2, 0, 0);
        displayOp(g, 1, 3, 3, 2, 0);
        break;
    case 10:
        displayOp(g, 6, 2, 2, 0, 1);
        displayOp(g, 5, 1, 2, 1, 0);
        displayOp(g, 4, 2, 3, 1, 0);
        displayOp(g, 3, 3, 1, 0, 0);
        displayOp(g, 2, 3, 2, 0, 0);
        displayOp(g, 1, 3, 3, 2, 0);
        break;
    case 11:
        displayOp(g, 6, 3, 2, 7, 0);
        displayOp(g, 5, 2, 2, 0, 0);
        displayOp(g, 4, 1, 2, 1, 0);
        displayOp(g, 3, 2, 3, 6, 0);
        displayOp(g, 2, 4, 2, 0, 1);
        displayOp(g, 1, 4, 3, 2, 0);
        break;
    case 12:
        displayOp(g, 6, 3, 2, 7, 1);
        displayOp(g, 5, 2, 2, 0, 0);
        displayOp(g, 4, 1, 2, 1, 0);
        displayOp(g, 3, 2, 3, 6, 0);
        displayOp(g, 2, 4, 2, 0, 0);
        displayOp(g, 1, 4, 3, 2, 0);
        break;
    case 13:
        displayOp(g, 6, 4, 1, 7, 1);
        displayOp(g, 5, 3, 1, 0, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 2, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 14:
        displayOp(g, 6, 4, 1, 7, 0);
        displayOp(g, 5, 3, 1, 0, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 2, 0);
        displayOp(g, 2, 2, 2, 0, 4);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 15:
        displayOp(g, 6, 4, 1, 0, 1);
        displayOp(g, 5, 4, 2, 7, 0);
        displayOp(g, 4, 3, 1, 0, 0);
        displayOp(g, 3, 3, 2, 0, 0);
        displayOp(g, 2, 2, 2, 1, 0);
        displayOp(g, 1, 3, 3, 0, 0);
        break;
    case 16:
        displayOp(g, 6, 4, 1, 0, 0);
        displayOp(g, 5, 4, 2, 7, 0);
        displayOp(g, 4, 3, 1, 0, 0);
        displayOp(g, 3, 3, 2, 0, 0);
        displayOp(g, 2, 2, 2, 1, 4);
        displayOp(g, 1, 3, 3, 0, 0);
        break;
    case 17:
        displayOp(g, 6, 4, 0, 0, 0);
        displayOp(g, 5, 4, 1, 0, 0);
        displayOp(g, 4, 4, 2, 7, 0);
        displayOp(g, 3, 3, 2, 0, 4);
        displayOp(g, 2, 2, 2, 1, 0);
        displayOp(g, 1, 3, 3, 0, 0);
        break;
    case 18:
        displayOp(g, 6, 3, 2, 3, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 2, 1, 0, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 19:
        displayOp(g, 6, 4, 2, 0, 0);
        displayOp(g, 5, 3, 2, 1, 0);
        displayOp(g, 4, 4, 3, 2, 0);
        displayOp(g, 3, 1, 2, 3, 1);
        displayOp(g, 2, 2, 3, 6, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 20:
        displayOp(g, 6, 3, 2, 3, 0);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 1, 2, 3, 1);
        displayOp(g, 2, 2, 3, 1, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 21:
        displayOp(g, 6, 3, 2, 4, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 2, 3, 1, 0);
        displayOp(g, 2, 1, 2, 0, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 22:
        displayOp(g, 6, 3, 2, 3, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 2, 2, 0, 0);
        displayOp(g, 2, 2, 3, 1, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 23:
        displayOp(g, 6, 3, 2, 4, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 2, 3, 1, 0);
        displayOp(g, 2, 1, 3, 1, 0);
        displayOp(g, 1, 0, 3, 1, 0);
        break;
    case 24:
        displayOp(g, 6, 3, 2, 3, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 2, 3, 1, 0);
        displayOp(g, 2, 1, 3, 1, 0);
        displayOp(g, 1, 0, 3, 1, 0);
        break;
    case 25:
        displayOp(g, 6, 4, 2, 0, 1);
        displayOp(g, 5, 3, 2, 1, 0);
        displayOp(g, 4, 4, 3, 2, 0);
        displayOp(g, 3, 2, 2, 0, 0);
        displayOp(g, 2, 2, 3, 6, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 26:
        displayOp(g, 6, 4, 2, 0, 0);
        displayOp(g, 5, 3, 2, 1, 0);
        displayOp(g, 4, 4, 3, 2, 0);
        displayOp(g, 3, 2, 2, 0, 1);
        displayOp(g, 2, 2, 3, 6, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 27:
        displayOp(g, 6, 4, 3, 2, 0);
        displayOp(g, 5, 3, 1, 0, 1);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 1, 0);
        displayOp(g, 2, 2, 2, 0, 0);
        displayOp(g, 1, 2, 3, 1, 0);
        break;
    case 28:
        displayOp(g, 6, 4, 2, 0, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 1, 0);
        displayOp(g, 2, 2, 3, 1, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 29:
        displayOp(g, 6, 4, 3, 2, 0);
        displayOp(g, 5, 3, 1, 0, 1);
        displayOp(g, 4, 3, 2, 0, 0);
        displayOp(g, 3, 3, 3, 1, 0);
        displayOp(g, 2, 2, 3, 1, 0);
        displayOp(g, 1, 1, 3, 1, 0);
        break;
    case 30:
        displayOp(g, 6, 4, 2, 0, 1);
        displayOp(g, 5, 4, 3, 2, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 2, 3, 1, 0);
        displayOp(g, 2, 1, 3, 1, 0);
        displayOp(g, 1, 0, 3, 1, 0);
        break;
    case 31:
        displayOp(g, 6, 5, 3, 2, 1);
        displayOp(g, 5, 4, 3, 1, 0);
        displayOp(g, 4, 3, 3, 1, 0);
        displayOp(g, 3, 2, 3, 1, 0);
        displayOp(g, 2, 1, 3, 1, 0);
        displayOp(g, 1, 0, 3, 1, 0);
        break;
    default:
        break;
    }
}