Test jagload is deprecated.

This test uses an osg::NodeVisitor to walk an OSG scene graph
and construct a jag draw graph.

As of v0.01.13, this test should build with dynamic JAG.

It will not work with static JAG because Osg2Jag is defined
multiple times (in this test, and also in the jagp-osgModel
plugin). This problem could be fixed, but this is not the
first maintenance issue encountered by this test, so the
decidion was made 11 March 2014 to remove it from the build.
