#include "context.h"

#include <QDirIterator>
#include <QTextStream>
#include <QDateTime>
#include <QScreen>
#include <QWindow>
#include <QDebug>
#include <QFile>
#include <QDir>

#include <QMessageBox>
#include <QFileDialog>
#include <QGuiApplication>

Context context;

Context::Context()
{

}

Context::~Context()
{
    delete canvas();
}

void Context::init()
{
    color1 = QColor("#009de0");
    color2 = QColor("#c20d19");
    brushSize = 5;
    eraserSize = 50;
    isWindowVisible = false;
    isMenuVisible = false;
    backgroundType = 0;
    tempFolder = "/tmp";
    openFolder = ".";
    currentPage = 0;
    numberOfPages = 0;
    pageHasChanges = false;
    notebookHasChanges = false;
    canvas = new QPixmap();
    canvasSize = QSize(1920, 1080);
    tempFolder = tempDir.path();
    screen = nullptr;

    QDir path1(QDir::homePath() + "/MEGA");
    QDir path2(QDir::homePath() + "/Dropbox");
    QDir path3("/mnt/warehouse2/MEGA");
    QDir path4(QDir::homePath() + "/Documents");
    QString path;

    if (path1.exists())
        path = path1.absolutePath() + "/Notebooks";

    else if (path2.exists())
        path = path2.absolutePath() + "/Notebooks";

    else if (path3.exists())
        path = path3.absolutePath() + "/Notebooks";

    else if (path4.exists())
        path = path4.absolutePath() + "/Notebooks";

    if (path.isEmpty())
        path = QDir::homePath() + "/Documents/Notebooks";

    QDir(path).mkpath(".");

    notebooksPath = path;
}

void Context::setColor(QColor color)
{
    color2 = color1();
    color1 = color;
}

void Context::setBrushSize(int size)
{
    brushSize = size;
}

void Context::setEraserSize(int size)
{
    eraserSize = size;
}

void Context::setMenuPosition(QPoint position)
{
    menuPosition = position;
}

void Context::setMenuVisible(bool visible)
{
    isMenuVisible = visible;
}

void Context::setWindowVisible(bool value)
{
    isWindowVisible = value;
}

void Context::toggleWindowVisibility()
{
    isWindowVisible = !isWindowVisible();
}

void Context::toggleMenuVisibility()
{
    context.setMenuVisible(!context.isMenuVisible());
}

void Context::toggleBackgroundType()
{
    backgroundType = backgroundType() == 0 ? 1 : 0;
}

void Context::setBackgroundType(int type)
{
    backgroundType = type;
}

void Context::setCanvasSize(QSize size)
{
    canvasSize = size;
}

void Context::undo()
{

}

void Context::redo()
{

}

void Context::clearPage()
{
    canvas.mValue->fill(Qt::transparent);
    canvas.notify();
    pageHasChanges = true;
}

void Context::previousPage()
{
    if (currentPage() != 0)
    {
        int64_t const newPage = currentPage() - 1;
        refreshCanvas(currentPage(), newPage);
        currentPage = newPage;
    }
}

void Context::nextPage()
{
    if (currentPage() != mPages.size())
    {
        int64_t const newPage = currentPage() + 1;
        refreshCanvas(currentPage(), newPage);
        currentPage = newPage;
    }
}

void Context::firstPage()
{
    if (currentPage() != 0)
    {
        int64_t const newPage = 0;
        refreshCanvas(currentPage(), newPage);
        currentPage = newPage;
    }
}

void Context::lastPage()
{
    if (currentPage() < mPages.size() - 1)
    {
        int64_t const newPage = mPages.size() - 1;
        refreshCanvas(currentPage(), newPage);
        currentPage = newPage;
    }

    else if (currentPage() == mPages.size() - 1)
    {
        int64_t const newPage = mPages.size();
        refreshCanvas(currentPage(), newPage);
        currentPage = newPage;
    }
}

void Context::deletePage()
{
    if (currentPage() < mPages.size())
    {
        mRemovedPages.push_back(mPages[currentPage()]);
        mPages.removeAt(currentPage());
        refreshCanvas(mPages.size(), currentPage());
        numberOfPages = mPages.size();
    }
}

void Context::setHasChanges()
{
    if (!notebookHasChanges())
        notebookHasChanges = true;

    if (!pageHasChanges())
        pageHasChanges = true;

    if (currentPage() == mPages.size())
    {
        int64_t timestamp = QDateTime::currentMSecsSinceEpoch();
//        qInfo() << "Registering current page as a new page in mPage as " << timestamp;
        mPages.push_back(timestamp);
        numberOfPages = mPages.size();
    }
}

void Context::newNotebook()
{
    mPages.clear();
    mRemovedPages.clear();

    notebookHasChanges = false;
    numberOfPages = 0;
    currentPage = 0;
    openFolder = ".";

    refreshCanvas(0, 0);
}

void Context::openNotebook(QString folderPath)
{
    mPages.clear();
    mRemovedPages.clear();

    QDirIterator it(folderPath);

    while (it.hasNext())
    {
        bool ok;
        QFileInfo filePath(it.next());
        Page page = filePath.baseName().toLongLong(&ok);

        if (ok)
            mPages.push_back(page);
    }

    notebookHasChanges = false;
    pageHasChanges = false;
    numberOfPages = mPages.size();
    currentPage = 0;
    openFolder = folderPath;

    refreshCanvas(0, 0);
}

void Context::saveNotebook()
{
    if (openFolder() == ".")
        return;

    QDir dir(openFolder());

    if (!dir.exists())
        dir.mkpath(".");

    if (pageHasChanges())
    {
        QString dst;
        int page = currentPage();
        int64_t const timestamp = (page < mPages.size() ? mPages[page] : 0);
        QTextStream(&dst) << tempFolder() << "/" << timestamp << ".png";
        canvas()->save(dst);
//        qInfo() << "Saved current page in temp folder as: " << dst;
    }

    for (int64_t & p : mPages)
    {
        QString src;
        QString dst;

        QTextStream(&src) << tempFolder() << "/" << p << ".png";
        QTextStream(&dst) << openFolder() << "/" << p << ".png";

        if (!QFile::exists(src))
            continue;

        if (QFile::exists(dst))
            QFile::remove(dst);

        QFile::copy(src, dst);
    }

    for (auto & p : mRemovedPages)
    {
        QString dst;

        QTextStream(&dst) << openFolder() << "/" << p << ".png";

        if (QFile::exists(dst))
            QFile::remove(dst);
    }

    mRemovedPages.clear();
    notebookHasChanges = false;
    pageHasChanges = false;
}

void Context::saveNotebookAs(QString folderPath)
{
//    qInfo() << " --- Starting save as ---";

    QDir folder(folderPath);

    if (folder.exists())
        folder.removeRecursively();

    folder.mkpath(".");

    if (pageHasChanges())
    {
        QString dst;
        int page = currentPage();
        int64_t const timestamp = (page < mPages.size() ? mPages[page] : 0);
        QTextStream(&dst) << tempFolder() << "/" << timestamp << ".png";
        canvas()->save(dst);
//        qInfo() << "Saved current page in temp folder as: " << dst;
    }

//    qInfo("mPages has %d elements", mPages.size());

    for (auto & p : mPages)
    {
        QString src1;
        QString src2;
        QString dst;

        QTextStream(&src1) << tempFolder() << "/" << p << ".png";
        QTextStream(&src2) << openFolder() << "/" << p << ".png";
        QTextStream(&dst) << folderPath << "/" << p << ".png";

        if (QFile::exists(src1))
        {
//            qInfo() << "Copying from temp folder to new folder " << p;
            QFile::copy(src1, dst);
        }

        else if (QFile::exists(src2))
        {
//            qInfo() << "Copying from open folder to new folder " << p;
            QFile::copy(src2, dst);
        }

        else
        {
            qWarning() << "Could not find page in " << src1 << " or " << src2;
            qWarning() << "Missing output page " << dst;
        }
    }

    openFolder = folderPath;
    mRemovedPages.clear();
    notebookHasChanges = false;
    pageHasChanges = false;
}

void Context::refreshCanvas(int previous, int current)
{
    QString src1;
    QString src2;
    QString dst;

    int64_t const timestampDst = (previous < mPages.size() ? mPages[previous] : 0);
    int64_t const timestampSrc = (current < mPages.size() ? mPages[current] : 0);

    QTextStream(&dst) << tempFolder() << "/" << timestampDst << ".png";
    QTextStream(&src1) << tempFolder() << "/" << timestampSrc << ".png";
    QTextStream(&src2) << openFolder() << "/" << timestampSrc << ".png";

//    qInfo() << "Refreshing canvas:" << "\tdst:" << dst << "\tsrc1:" << src1 << "\tsrc2:" << src2;

    if (timestampDst != 0 && pageHasChanges())
    {
//        qInfo() << "Page had changes, saving them to " << dst;
        canvas()->save(dst);
    }

    if (timestampSrc != 0 && QFile::exists(src1))
    {
//        qInfo() << "Loading next canvas from src1 = " << src1;
        canvas.mValue->load(src1);
        canvas.notify();
    }

    else if (timestampSrc != 0 && QFile::exists(src2))
    {
//        qInfo() << "Loading next canvas from src2 = " << src2;
        canvas.mValue->load(src2);
        canvas.notify();
    }

    else
    {
//        qInfo() << "Creating next canvas from scratch";
        *canvas.mValue = QPixmap(canvasSize());
        canvas.mValue->fill(Qt::transparent);
        canvas.notify();
    }

    pageHasChanges = false;
}

void Context::newNotebookSafe(QWidget * parent)
{
    int ret = QMessageBox::No;

    if (context.notebookHasChanges())
    {
        QMessageBox msgBox;
        msgBox.setText("You have unsaved changes. Do you want to save them?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        ret = msgBox.exec();
    }

    if (ret == QMessageBox::Cancel)
        return;

    if (ret == QMessageBox::Yes)
    {
        if (context.openFolder() == ".")
        {
            QString dir = QFileDialog::getSaveFileName(
                        parent,
                        parent->tr("Save notebook as"),
                        context.notebooksPath());

            if (dir.isEmpty())
                return;

            context.saveNotebookAs(dir);
        }
        else
        {
            context.saveNotebook();
        }
    }

    context.newNotebook();
}

void Context::openNotebookSafe(QWidget * parent)
{
    int ret = QMessageBox::No;

    if (context.notebookHasChanges())
    {
        QMessageBox msgBox;
        msgBox.setText("You have unsaved changes. Do you want to save your current work?");
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
        ret = msgBox.exec();
    }

    if (ret == QMessageBox::Cancel)
        return;

    if (ret == QMessageBox::Yes)
    {
        if (context.openFolder() == ".")
        {
            QString dir = QFileDialog::getSaveFileName(
                        parent,
                        parent->tr("Save notebook as"),
                        context.notebooksPath());

            if (dir.isEmpty())
                return;

            context.saveNotebookAs(dir);
        }
        else
        {
            context.saveNotebook();
        }
    }

    QString dir = QFileDialog::getExistingDirectory(
                parent,
                parent->tr("Open notebook"),
                context.notebooksPath(),
                QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (dir.isEmpty())
        return;

    context.openNotebook(dir);
}

void Context::saveNotebookSafe(QWidget * parent)
{
    if (context.openFolder() == ".")
    {
        QString dir = QFileDialog::getSaveFileName(
                    parent,
                    parent->tr("Save notebook as"),
                    context.notebooksPath());

        if (dir.isEmpty())
            return;

        context.saveNotebookAs(dir);
    }
    else
    {
        context.saveNotebook();
    }
}

void Context::saveNotebookAsSafe(QWidget * parent)
{

}

void Context::exportNotebook(QWidget * parent)
{

}
