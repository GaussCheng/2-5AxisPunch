///////////////////////////////////////////////////////////
//  icmainframe.h
//  Implementation of the Class ICMainFrame
//  Created on:      21-七月-2011 19:00:53
//  Original author: GaussCheng
///////////////////////////////////////////////////////////

#if !defined(EA_D3217A25_EB98_41e5_85A9_B9D941394EE3__INCLUDED_)
#define EA_D3217A25_EB98_41e5_85A9_B9D941394EE3__INCLUDED_


#include <QWidget>
class ICDefaultScreenSaver;
class  ScreenFunctionObject: public QObject
{
    Q_OBJECT
public:
    void ScreenSave() { emit ScreenSaved();}
    void ScreenRestore() { emit ScreenRestored();}

Q_SIGNALS:
    void ScreenSaved();
    void ScreenRestored();
};

/**
 * 工控页面的主框架，提供工控UI系统的基础设施，包括如下：
 * <ol>
 * <li>屏保机制</li>
 * </ol>
 * @author GaussCheng
 * @version 1.0 \return
 * @updated 23-七月-2011 8:41:58
 */
class  ICMainFrame:public QWidget
{
    Q_OBJECT
public:
    ICMainFrame(QWidget* parent = NULL);
	virtual ~ICMainFrame();
	static void SetScreenSaverInterval(int intervals);

Q_SIGNALS:
    void ScreenSave();
    void ScreenRestore();
protected:


#ifdef Q_WS_QWS
    ICDefaultScreenSaver* screenSaver_;
#endif
};
#endif // !defined(EA_D3217A25_EB98_41e5_85A9_B9D941394EE3__INCLUDED_)
