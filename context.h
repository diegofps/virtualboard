#ifndef CONTEXT_H
#define CONTEXT_H

#include <QVector>
#include <QColor>
#include <QPixmap>
#include <QScreen>
#include <QTemporaryDir>

class Context;

template <typename T>
class ContextListener
{
public:

    void * idd;

    std::function<void(T const &)> callback;

public:

    ContextListener(void * const idd) :
        idd(idd), callback(nullptr) { }

    ContextListener(void * const idd, std::function<void(const T)> func) :
        idd(idd), callback(func) { }

    bool operator==(const ContextListener & other) const
    { return other.idd == idd; }

};

template <typename T>
class Parameter
{
private:

    QVector<ContextListener<T>> mListeners;

    T mValue;

public:

    Parameter()
    {

    }

    Parameter(T value) :
        mValue(value)
    {

    }

    const T & operator()()
    {
        return mValue;
    }

    void addListener(void * const idd, std::function<void(T const &)> listener)
    {
        ContextListener<T> tmp(idd, listener);
        if (!mListeners.contains(tmp))
            mListeners.push_back(tmp);
        listener(mValue);
    }

    void removeListener(void * const idd)
    {
        mListeners.erase(ContextListener<T>(idd));
    }

private:

    friend class Context;

    void operator=(T newValue)
    {
        mValue = newValue;
        notify();
    }

    void notify() const
    {
        for (auto & listener : mListeners)
            listener.callback(mValue);
    }

};

typedef int VoidEvent;

template <typename T>
class Event
{
private:

    QVector<ContextListener<T>> mListeners;

public:

    void send(T & event)
    {
        for (auto & l : mListeners)
            l.callback(event);
    }

    void addListener(void * idd, std::function<void(T const &)> listener)
    {
        ContextListener<T> tmp(idd, listener);
        if (!mListeners.contains(tmp))
            mListeners.push_back(tmp);
    }

    void removeListener(void * idd)
    {
        mListeners.erase(ContextListener<T>(idd));
    }

};



class Context : public QObject
{
    Q_OBJECT

private:

    typedef int64_t Page;

private:

    QVector<Page> mPages;

    QVector<Page> mRemovedPages;

    QTemporaryDir tempDir;

    QScreen * screen;

public:

    Context();

    ~Context();

    void init();

    // Parameters

    Parameter<QColor> color1;
    Parameter<QColor> color2;

    Parameter<int> brushSize;
    Parameter<int> eraserSize;

    Parameter<bool> isWindowVisible;
    Parameter<bool> isMenuVisible;
    Parameter<QPoint> menuPosition;

    Parameter<int> backgroundType;

    Parameter<QString> tempFolder;
    Parameter<QString> openFolder;

    Parameter<int> currentPage;
    Parameter<int> numberOfPages;
    Parameter<bool> pageHasChanges;
    Parameter<bool> notebookHasChanges;

    Parameter<QPixmap*> canvas;
    Parameter<QSize> canvasSize;

    Parameter<QString> notebooksPath;

public slots:

    // Parameter modifiers

    void setColor(QColor color);

    void setBrushSize(int size);

    void setEraserSize(int size);

    void setMenuVisible(bool visible);

    void setMenuPosition(QPoint position);

    void setWindowVisible(bool value);

    void setBackgroundType(int type);

    void setCanvasSize(QSize size);

    void toggleWindowVisibility();

    void toggleMenuVisibility();

    void toggleBackgroundType();

    void undo();

    void redo();

    void clearPage();

    void nextPage();

    void previousPage();

    void firstPage();

    void lastPage();

    void deletePage();

    void setHasChanges();

    void newNotebook();

    void openNotebook(QString folderPath);

    void saveNotebook();

    void saveNotebookAs(QString folderPath);

    void newNotebookSafe(QWidget *parent);

    void openNotebookSafe(QWidget *parent);

    void saveNotebookSafe(QWidget *parent);

    void saveNotebookAsSafe(QWidget *parent);

    void exportNotebook(QWidget *parent);

public:

    // Events

private:

    void refreshCanvas(int previous, int current);

};

extern Context context;

#endif // CONTEXT_H
