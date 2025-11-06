#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

    ofSetFrameRate(60);
    ofSetBackgroundColor(ofColor::blue);
    backgroundImage.load("background.png");
    backgroundImage.resize(ofGetWindowWidth(), ofGetWindowHeight());

    // Musics / Sounds
    // Loads background music
    backgroundMusic.load("sfx/Aqua_Ambience.mp3");
    backgroundMusic.setLoop(true);
    backgroundMusic.play();

    // BONUS - Themes
    background2Image.load("background2.png");
    background2Image.resize(ofGetWindowWidth(), ofGetWindowHeight());

    background3Image.load("background3.png");
    background3Image.resize(ofGetWindowWidth(), ofGetWindowHeight());

    background4Image.load("background4.png");
    background4Image.resize(ofGetWindowWidth(), ofGetWindowHeight());

    background2Music.load("sfx/Green_Ambience.mp3");
    background2Music.setLoop(true);
    background2Music.setVolume(0.3); // pretty loud

    background3Music.load("sfx/Crafty_Ambience.mp3");
    background3Music.setLoop(true);

    background4Music.load("sfx/Coral_Ambience.mp3");
    background4Music.setLoop(true);
    background4Music.setVolume(0.5);

    // Loads bite sound
    biteSoundEffect.load("sfx/Bite.mp3");
    biteSoundEffect.setMultiPlay(true); // allows it to play multiple times without interruption
    biteSoundEffect.setVolume(0.4);

    // Load error sound
    errorSoundEffect.load("sfx/Error.mp3");
    errorSoundEffect.setVolume(1.5);


    std::shared_ptr<Aquarium> myAquarium;
    std::shared_ptr<PlayerCreature> player;

    // make the game scene manager 
    gameManager = std::make_unique<GameSceneManager>();


    // first we make the intro scene 
    gameManager->AddScene(std::make_shared<GameIntroScene>(
        GameSceneKindToString(GameSceneKind::GAME_INTRO),
        std::make_shared<GameSprite>("title.png", ofGetWindowWidth(), ofGetWindowHeight())
    ));

    //AquariumSpriteManager
    spriteManager = std::make_shared<AquariumSpriteManager>();

    // Lets setup the aquarium
    myAquarium = std::make_shared<Aquarium>(ofGetWindowWidth(), ofGetWindowHeight(), spriteManager);
    player = std::make_shared<PlayerCreature>(ofGetWindowWidth()/2 - 50, ofGetWindowHeight()/2 - 50, DEFAULT_SPEED, this->spriteManager->GetSprite(AquariumCreatureType::NPCreature));
    player->setDirection(0, 0); // Initially stationary
    player->setBounds(ofGetWindowWidth() - 20, ofGetWindowHeight() - 20);


    myAquarium->addAquariumLevel(std::make_shared<Level_0>(0, 10));
    myAquarium->addAquariumLevel(std::make_shared<Level_1>(1, 15));
    myAquarium->addAquariumLevel(std::make_shared<Level_2>(2, 20));
    myAquarium->addAquariumLevel(std::make_shared<Level_3>(3, 75));  //level 3 added
    myAquarium->addAquariumLevel(std::make_shared<Level_4>(4, 150)); //level 4 added

    myAquarium->Repopulate(); // initial population

    // now that we are mostly set, lets pass the player and the aquarium downstream
    gameManager->AddScene(std::make_shared<AquariumGameScene>(
        std::move(player), std::move(myAquarium), GameSceneKindToString(GameSceneKind::AQUARIUM_GAME), biteSoundEffect, errorSoundEffect
    )); // player and aquarium are owned by the scene moving forward

    // Load font for game over message
    gameOverTitle.load("Verdana.ttf", 12, true, true);
    gameOverTitle.setLineHeight(34.0f);
    gameOverTitle.setLetterSpacing(1.035);


    gameManager->AddScene(std::make_shared<GameOverScene>(
        GameSceneKindToString(GameSceneKind::GAME_OVER),
        std::make_shared<GameSprite>("game-over.png", ofGetWindowWidth(), ofGetWindowHeight())
    ));

    ofSetLogLevel(OF_LOG_NOTICE); // Set default log level
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(gameManager->GetActiveSceneName() == GameSceneKindToString(GameSceneKind::GAME_OVER)){
        return; // Stop updating if game is over or exiting
    }

    if(gameManager->GetActiveSceneName() == GameSceneKindToString(GameSceneKind::AQUARIUM_GAME)){
        auto gameScene = std::static_pointer_cast<AquariumGameScene>(gameManager->GetActiveScene());
        if(gameScene->GetLastEvent() != nullptr && gameScene->GetLastEvent()->isGameOver()){
            gameManager->Transition(GameSceneKindToString(GameSceneKind::GAME_OVER));
            return;
        }
        
    }

    // BONUS


    gameManager->UpdateActiveScene();
    


}

//--------------------------------------------------------------
void ofApp::draw(){
    switch(currentTheme) {
        case 0: // aquatic(default) theme
            backgroundImage.draw(0,0);
            break;
        case 1: // frutiger theme
            background2Image.draw(0,0);
            break;
        case 2: // minecraft theme
            background3Image.draw(0,0);
            break;
        case 3: // tropical theme
            background4Image.draw(0,0);
            break;
        default:
            backgroundImage.draw(0,0);
            break;
    }
    gameManager->DrawActiveScene();
}

//--------------------------------------------------------------
void ofApp::ApplyTheme(int i) {
    if(i < 0) i = 0;
    if(i > 3) i = 3;
    currentTheme = i;

    if(backgroundMusic.isPlaying()) backgroundMusic.stop();
    if(background2Music.isPlaying()) background2Music.stop();
    if(background3Music.isPlaying()) background3Music.stop();
    if(background4Music.isPlaying()) background4Music.stop();

    switch(currentTheme) {
        case 0: // aquatic(default) theme
            backgroundMusic.play();
            break;
        case 1: // frutiger theme
            background2Music.play();
            break;
        case 2: // minecraft theme
            background3Music.play();
            break;
        case 3: // tropical theme
            background4Music.play();
            break;
        default:
            backgroundMusic.play();
            break;
    }

}
//--------------------------------------------------------------
void ofApp::exit(){
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (lastEvent.isGameExit()) { 
        ofLogNotice() << "Game has ended. Press ESC to exit." << std::endl;
        return; // Ignore other keys after game over
    }
    if(gameManager->GetActiveSceneName() == GameSceneKindToString(GameSceneKind::AQUARIUM_GAME)){
        auto gameScene = std::static_pointer_cast<AquariumGameScene>(gameManager->GetActiveScene());
        switch(key){
            case OF_KEY_UP:
                gameScene->GetPlayer()->setDirection(gameScene->GetPlayer()->isXDirectionActive()?gameScene->GetPlayer()->getDx():0, -1);
                break;
                case OF_KEY_DOWN:
                gameScene->GetPlayer()->setDirection(gameScene->GetPlayer()->isXDirectionActive()?gameScene->GetPlayer()->getDx():0, 1);
                break;
            case OF_KEY_LEFT:
                gameScene->GetPlayer()->setDirection(-1, gameScene->GetPlayer()->isYDirectionActive()?gameScene->GetPlayer()->getDy():0);
                gameScene->GetPlayer()->setFlipped(true);
                break;
                case OF_KEY_RIGHT:
                gameScene->GetPlayer()->setDirection(1, gameScene->GetPlayer()->isYDirectionActive()?gameScene->GetPlayer()->getDy():0);
                gameScene->GetPlayer()->setFlipped(false);
                break;
            default:
                break;
        }

    // BONUS
    if(key >= '1' && key <= '4') {
        ApplyTheme(key - '1');
        return; // allows you to press from 1 to 4 for the themes
    }
    
        
    
        gameScene->GetPlayer()->move();
        return;

    }

    if(gameManager->GetActiveSceneName() == GameSceneKindToString(GameSceneKind::GAME_INTRO)){
        switch (key)
        {
        case ' ':
            gameManager->Transition(GameSceneKindToString(GameSceneKind::AQUARIUM_GAME));
            break;
        
        default:
            break;
        }
    }


    
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    if(gameManager->GetActiveSceneName() == GameSceneKindToString(GameSceneKind::AQUARIUM_GAME)){
        auto gameScene = std::static_pointer_cast<AquariumGameScene>(gameManager->GetActiveScene());
    if( key == OF_KEY_UP || key == OF_KEY_DOWN){
        gameScene->GetPlayer()->setDirection(gameScene->GetPlayer()->isXDirectionActive()?gameScene->GetPlayer()->getDx():0, 0);
        gameScene->GetPlayer()->move();
        return;
    }
    
    if(key == OF_KEY_LEFT || key == OF_KEY_RIGHT){
        gameScene->GetPlayer()->setDirection(0, gameScene->GetPlayer()->isYDirectionActive()?gameScene->GetPlayer()->getDy():0);
        gameScene->GetPlayer()->move();
        return;
    }

    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

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
void ofApp::mouseScrolled(int x, int y, float scrollX, float scrollY){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    backgroundImage.resize(w, h);
    background2Image.resize(w, h);
    background3Image.resize(w, h);
    background4Image.resize(w, h);
    auto aquariumScene = std::static_pointer_cast<AquariumGameScene>(gameManager->GetScene(GameSceneKindToString(GameSceneKind::AQUARIUM_GAME)));
    aquariumScene->GetAquarium()->setBounds(w,h);
    aquariumScene->GetPlayer()->setBounds(w - 20, h - 20);

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
