// Internal Includes

// Library/third-party includes

// Standard includes



class GloveUpdater : public OSG::Node callback
{
	public:
		GloveUpdater(Glove &g): _glove(g);
		callback(osg:node*); //update finger bend xforms
}