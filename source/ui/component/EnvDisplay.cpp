#include "EnvDisplay.h"
#include "../DXLookNFeel.h"
#include "parameter/Model.h"
#include "msfa/pitchenv.h"

static float EG_rate_rise_duration[128] = {
    38.00000 ,34.96000 ,31.92000 ,28.88000 ,25.84000 ,
    22.80000 ,20.64000 ,18.48000 ,16.32000 ,14.16000 ,
    12.00000 ,11.10000 ,10.20000 ,9.30000 ,8.40000 ,
    7.50000 ,6.96000 ,6.42000 ,5.88000 ,5.34000 ,
    4.80000 ,4.38000 ,3.96000 ,3.54000 ,3.12000 ,
    2.70000 ,2.52000 ,2.34000 ,2.16000 ,1.98000 ,
    1.80000 ,1.70000 ,1.60000 ,1.50000 ,1.40000 ,
    1.30000 ,1.22962 ,1.15925 ,1.08887 ,1.01850 ,
    0.94813 ,0.87775 ,0.80737 ,0.73700 ,0.69633 ,
    0.65567 ,0.61500 ,0.57833 ,0.54167 ,0.50500 ,
    0.47300 ,0.44100 ,0.40900 ,0.37967 ,0.35033 ,
    0.32100 ,0.28083 ,0.24067 ,0.20050 ,0.16033 ,
    0.12017 ,0.08000 ,0.07583 ,0.07167 ,0.06750 ,
    0.06333 ,0.05917 ,0.05500 ,0.04350 ,0.03200 ,
    0.02933 ,0.02667 ,0.02400 ,0.02200 ,0.02000 ,
    0.01800 ,0.01667 ,0.01533 ,0.01400 ,0.01300 ,
    0.01200 ,0.01100 ,0.01000 ,0.00900 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00767 ,0.00733 ,
    0.00700 ,0.00633 ,0.00567 ,0.00500 ,0.00433 ,
    0.00367 ,0.00300 ,0.00300 ,0.00300 ,0.00300 ,
    0.00300 ,0.00300 ,0.00300 ,0.00300 ,0.00300 ,
    0.00300 ,0.00300 ,0.00300 ,0.00300 ,0.00300 ,
    0.00300 ,0.00300 ,0.00300 ,0.00300 ,0.00300 ,
    0.00300 ,0.00300 ,0.00300 ,0.00300 ,0.00300 ,
    0.00300 ,0.00300 ,0.00300 ,0.00300 ,0.00300 ,
    0.00300 ,0.00300 ,0.00300
};

static float EG_rate_decay_duration[128] = {
    318.00000 ,283.75000 ,249.50000 ,215.25000 ,181.00000 ,
    167.80000 ,154.60001 ,141.39999 ,128.20000 ,115.00000 ,
    104.60000 ,94.20000 ,83.80000 ,73.40000 ,63.00000 ,
    58.34000 ,53.68000 ,49.02000 ,44.36000 ,39.70000 ,
    35.76000 ,31.82000 ,27.88000 ,23.94000 ,20.00000 ,
    18.24000 ,16.48000 ,14.72000 ,12.96000 ,11.20000 ,
    10.36000 ,9.52000 ,8.68000 ,7.84000 ,7.00000 ,
    6.83250 ,6.66500 ,6.49750 ,6.33000 ,6.16250 ,
    5.99500 ,5.82750 ,5.66000 ,5.10000 ,4.54000 ,
    3.98000 ,3.64833 ,3.31667 ,2.98500 ,2.65333 ,
    2.32167 ,1.99000 ,1.77333 ,1.55667 ,1.34000 ,
    1.22333 ,1.10667 ,0.99000 ,0.89667 ,0.80333 ,
    0.71000 ,0.65000 ,0.59000 ,0.53000 ,0.47000 ,
    0.41000 ,0.32333 ,0.23667 ,0.15000 ,0.12700 ,
    0.10400 ,0.08100 ,0.07667 ,0.07233 ,0.06800 ,
    0.06100 ,0.05400 ,0.04700 ,0.04367 ,0.04033 ,
    0.03700 ,0.03300 ,0.02900 ,0.02500 ,0.02333 ,
    0.02167 ,0.02000 ,0.01767 ,0.01533 ,0.01300 ,
    0.01133 ,0.00967 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800 ,0.00800 ,0.00800 ,
    0.00800 ,0.00800 ,0.00800
};

static float EG_rate_decay_percent[128] = {
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00501 ,0.01001 ,0.01500 ,
    0.02000 ,0.02800 ,0.03600 ,0.04400 ,0.05200 ,
    0.06000 ,0.06800 ,0.07600 ,0.08400 ,0.09200 ,
    0.10000 ,0.10800 ,0.11600 ,0.12400 ,0.13200 ,
    0.14000 ,0.15000 ,0.16000 ,0.17000 ,0.18000 ,
    0.19000 ,0.20000 ,0.21000 ,0.22000 ,0.23000 ,
    0.24000 ,0.25100 ,0.26200 ,0.27300 ,0.28400 ,
    0.29500 ,0.30600 ,0.31700 ,0.32800 ,0.33900 ,
    0.35000 ,0.36500 ,0.38000 ,0.39500 ,0.41000 ,
    0.42500 ,0.44000 ,0.45500 ,0.47000 ,0.48500 ,
    0.50000 ,0.52000 ,0.54000 ,0.56000 ,0.58000 ,
    0.60000 ,0.62000 ,0.64000 ,0.66000 ,0.68000 ,
    0.70000 ,0.73200 ,0.76400 ,0.79600 ,0.82800 ,
    0.86000 ,0.89500 ,0.93000 ,0.96500 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000

};

static float EG_rate_rise_percent[128] = {
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00001 ,0.00001 ,0.00001 ,
    0.00001 ,0.00001 ,0.00501 ,0.01001 ,0.01500 ,
    0.02000 ,0.02800 ,0.03600 ,0.04400 ,0.05200 ,
    0.06000 ,0.06800 ,0.07600 ,0.08400 ,0.09200 ,
    0.10000 ,0.10800 ,0.11600 ,0.12400 ,0.13200 ,
    0.14000 ,0.15000 ,0.16000 ,0.17000 ,0.18000 ,
    0.19000 ,0.20000 ,0.21000 ,0.22000 ,0.23000 ,
    0.24000 ,0.25100 ,0.26200 ,0.27300 ,0.28400 ,
    0.29500 ,0.30600 ,0.31700 ,0.32800 ,0.33900 ,
    0.35000 ,0.36500 ,0.38000 ,0.39500 ,0.41000 ,
    0.42500 ,0.44000 ,0.45500 ,0.47000 ,0.48500 ,
    0.50000 ,0.52000 ,0.54000 ,0.56000 ,0.58000 ,
    0.60000 ,0.62000 ,0.64000 ,0.66000 ,0.68000 ,
    0.70000 ,0.73200 ,0.76400 ,0.79600 ,0.82800 ,
    0.86000 ,0.89500 ,0.93000 ,0.96500 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000 ,1.00000 ,1.00000 ,
    1.00000 ,1.00000 ,1.00000
};

static double getDuration(int p_rate, int p_level_l, int p_level_r) {
    float *duration_table=(p_level_r>p_level_l) ? EG_rate_rise_duration : EG_rate_decay_duration;
    double duration=duration_table[p_rate];

    float* percent_table =(p_level_r>p_level_l) ? EG_rate_rise_percent  : EG_rate_decay_percent;
    duration *= fabs(percent_table[p_level_r]-percent_table[p_level_l]);
    return duration;
}

EnvDisplay::EnvDisplay(DexedApvts &apvts, int op) {
    for (int i=0; i<4; i++) {
        rates[i].referTo(apvts, IDs::egRate.op(op).idx(i).name);
        levels[i].referTo(apvts, IDs::egLevel.op(op).idx(i).name);
    }
}

void EnvDisplay::paint(juce::Graphics& g) {
    const float w = static_cast<float>(getWidth());
    const float h = static_cast<float>(getHeight());

    if (w <= 10.0f || h <= 10.0f)
        return;

    g.setColour(DXLookNFeel::background);
    g.fillRoundedRectangle(0.0f, 0.0f, w, h, 3.0f);

    double d[4];
    d[0] = getDuration(rates[0], levels[3], levels[0]);
    d[1] = getDuration(rates[1], levels[0], levels[1]);
    d[2] = getDuration(rates[2], levels[1], levels[2]);
    d[3] = getDuration(rates[3], levels[2], levels[3]);

    for (int i = 0; i < 4; ++i)
        d[i] = juce::jmax(0.04, d[i]);

    const double attackDecayTime = d[0] + d[1] + d[2];

    const double sustainPlateauTime = juce::jmax(0.2, attackDecayTime * 0.25);
    const double totalTime = attackDecayTime + sustainPlateauTime + d[3];

    const float padX = 4.0f;
    const float padY = 4.0f;
    const float innerW = w - (padX * 2.0f);
    const float innerH = h - (padY * 2.0f);

    auto timeToX = [&](double time) -> float {
        return padX + static_cast<float>((time / totalTime) * innerW);
    };

    auto levelToY = [&](int lvl) -> float {
        float normalized = juce::jlimit(0.0f, 99.0f, static_cast<float>(lvl)) / 99.0f;
        return padY + innerH * (1.0f - normalized);
    };

    juce::Point<float> p0(timeToX(0.0), levelToY(levels[3]));
    juce::Point<float> p1(timeToX(d[0]), levelToY(levels[0]));
    juce::Point<float> p2(timeToX(d[0] + d[1]), levelToY(levels[1]));
    juce::Point<float> p3(timeToX(attackDecayTime), levelToY(levels[2]));
    juce::Point<float> p4(timeToX(attackDecayTime + sustainPlateauTime), levelToY(levels[2]));
    juce::Point<float> p5(timeToX(totalTime), levelToY(levels[3]));


    juce::Path strokePath;
    strokePath.startNewSubPath(p0);
    strokePath.lineTo(p1);
    strokePath.lineTo(p2);
    strokePath.lineTo(p3);
    strokePath.lineTo(p4); 
    strokePath.lineTo(p5);  

    juce::Path fillPath = strokePath;
    const float bottomY = h - padY;
    fillPath.lineTo(p5.x, bottomY);
    fillPath.lineTo(p0.x, bottomY);
    fillPath.closeSubPath();

    juce::Colour accentColour = DXLookNFeel::fillColour;  
    g.setColour(accentColour.withAlpha(0.1f));
    g.fillPath(fillPath);

    g.setColour(accentColour);
    g.strokePath(strokePath, juce::PathStrokeType(1.8f, juce::PathStrokeType::curved,
                                                  juce::PathStrokeType::rounded));

    
    g.setFont(juce::Font(11.0f, juce::Font::bold));
    g.setColour(DXLookNFeel::textColour.withAlpha(0.75f));
    String len;
    len << static_cast<int>(vPos);
    g.drawText(len, 5, 2, 24, 14, juce::Justification::left, true);
}

PitchEnvDisplay::PitchEnvDisplay(DexedApvts &apvts) {
    for (int i=0; i<4; i++) {
        rates[i].referTo(apvts, IDs::pitchEgRate.idx(i).name);
        levels[i].referTo(apvts, IDs::pitchEgLevel.idx(i).name);
    }
}

void PitchEnvDisplay::paint(juce::Graphics& g) {
    const float w = static_cast<float>(getWidth());
    const float h = static_cast<float>(getHeight());

    if (w <= 10.0f || h <= 10.0f)
        return;

    g.setColour(DXLookNFeel::background);
    g.fillRoundedRectangle(0.0f, 0.0f, w, h, 3.0f);

 
    const float padX = 5.0f;
    const float padY = 5.0f;
    const float innerW = w - (padX * 2.0f);
    const float innerH = h - (padY * 2.0f);
    const float centerY = padY + innerH * 0.5f;

    g.setColour(DXLookNFeel::borderOutline);
    float dashes[] = {3.0f, 3.0f};
    g.drawDashedLine(juce::Line<float>(padX, centerY, padX + innerW, centerY), dashes, 2, 1.0f);

    float dist[4];
    float total = 0.0f;

    int oldVal = pitchenv_tab[levels[3]] + 128;

    for (int i = 0; i < 4; ++i) {
        int newVal = pitchenv_tab[levels[i]] + 128;
        float rate = static_cast<float>(juce::jmax(1, (int)pitchenv_rate[rates[i]]));
        dist[i] = static_cast<float>(std::abs(newVal - oldVal)) / rate;

        dist[i] = juce::jmax(0.12f, dist[i]);
        total += dist[i];
        oldVal = newVal;
    }

    if (total < 0.0001f) {
        dist[0] = dist[1] = dist[2] = dist[3] = 1.0f;
        total = 4.0f;
    }

    auto pitchToY = [&](int lvl) -> float {
        int rawVal = pitchenv_tab[lvl] + 128;                   
        float normalized = static_cast<float>(rawVal) / 255.0f;  
        normalized = juce::jlimit(0.0f, 1.0f, normalized);
        return padY + innerH * (1.0f - normalized);
    };

    juce::Point<float> nodes[5];
    nodes[0] = {padX, pitchToY(levels[3])};

    float currentX = padX;
    for (int i = 0; i < 4; ++i) {
        currentX += (dist[i] / total) * innerW;
        nodes[i + 1] = {currentX, pitchToY(levels[i])};
    }

    juce::Path strokePath;
    strokePath.startNewSubPath(nodes[0]);
    for (int i = 1; i < 5; ++i)
        strokePath.lineTo(nodes[i]);

    juce::Path fillPath = strokePath;
    fillPath.lineTo(nodes[4].x, centerY);
    fillPath.lineTo(nodes[0].x, centerY);
    fillPath.closeSubPath();

    juce::Colour accentColour = DXLookNFeel::fillColour;
    juce::ColourGradient fillGrad(accentColour.withAlpha(0.28f), 0.0f, padY,
                                  accentColour.withAlpha(0.04f), 0.0f, centerY, false);
    g.setGradientFill(fillGrad);
    g.fillPath(fillPath);

    g.setColour(accentColour);
    g.strokePath(strokePath, juce::PathStrokeType(1.8f, juce::PathStrokeType::curved,
                                                  juce::PathStrokeType::rounded));
}