//#include <SDL/SDL_gfxPrimitives.h>
//#define CIRCLE_X 0
//#define CIRCLE_Y 0
//
//
//void creer_cercle(SDL_Renderer *renderer, struct spritesheet *spritesheet) {
//    // Création d'une nouvelle texture pour le cercle
//    SDL_Texture *circle_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, 17, 17);
//
//    // Activation de la texture comme cible de rendu
//    SDL_SetRenderTarget(renderer, circle_texture);
//
//    // Dessin du cercle
//    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Couleur rouge
//    SDL_RenderClear(renderer);
//    filledCircleColor(renderer, 8, 8, 8, SDL_MapRGB(SDL_GetWindowSurface(SDL_GetWindowFromID(SDL_GetRendererOutputSize(renderer)->h)), 255, 0, 0)); // Dessin du cercle rempli
//
//    // Réinitialisation de la cible de rendu
//    SDL_SetRenderTarget(renderer, NULL);
//
//    // Ajout de la texture du cercle à la spritesheet
//    spritesheet_create(spritesheet, circle_texture, CIRCLE_X, CIRCLE_Y);
//}
