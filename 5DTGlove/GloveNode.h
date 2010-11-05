// Internal Includes

// Library/third-party includes

// Standard includes



class GloveNode : public OSG::Group
{
	public:
		GloveNode(Glove &g):
			_glove(g),
			_updater(new GloveUpdater(g))
		
	OSG::ref_ptr<GloveUpdater> _updater;
}