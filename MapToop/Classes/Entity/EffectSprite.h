#ifndef _EffectSprite_H_
#define _EffectSprite_H_
#include "Config/Head.h"
//
// EffectSprite
//

class Effect;
class EffectSprite : public Sprite
{
public:
	static EffectSprite *create(const std::string& filename);

	void setEffect(Effect* effect);
	void addEffect(Effect *effect, ssize_t order);

	void draw(Renderer *renderer, const Mat4 &transform, uint32_t flags) override;

protected:
	EffectSprite();
	~EffectSprite();

	std::vector<std::tuple<ssize_t, Effect*, QuadCommand>> _effects;
	Effect* _defaultEffect;
};


#endif //!EffectSprite