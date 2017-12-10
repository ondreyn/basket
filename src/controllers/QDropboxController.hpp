/*
 * QDropboxController.hpp
 *
 *  Created on: Nov 30, 2017
 *      Author: doctorrokter
 */

#ifndef QDROPBOXCONTROLLER_HPP_
#define QDROPBOXCONTROLLER_HPP_

#include <QObject>
#include <QStringList>
#include <QVariantList>
#include <QtGui/QImage>
#include "../qdropbox/QDropbox.hpp"
#include "../qdropbox/QDropboxFile.hpp"
#include "../qdropbox/Account.hpp"
#include "../qdropbox/QDropboxSpaceUsage.hpp"
#include "../Logger.hpp"
#include <bb/system/SystemToast>
#include "../util/FileUtil.hpp"

using namespace bb::system;

class QDropboxController: public QObject {
    Q_OBJECT
    Q_PROPERTY(QVariantList selected READ getSelected WRITE setSelected NOTIFY selectedChanged)
    Q_PROPERTY(QVariantList downloads READ getDownloads NOTIFY downloadsChanged)
    Q_PROPERTY(QVariantList uploads READ getUploads NOTIFY uploadsChanged)
public:
    QDropboxController(QDropbox* qdropbox, FileUtil* fileUtil, QObject* parent = 0);
    virtual ~QDropboxController();

    Q_INVOKABLE QString fullPath() const;
    Q_INVOKABLE QString topPath() const;
    Q_INVOKABLE QString popPath();
    Q_INVOKABLE void listFolder(const QString& path = "", const int& limit = 0);
    Q_INVOKABLE void listFolderContinue(const QString& cursor);
    Q_INVOKABLE void createFolder(const QString& path);
    Q_INVOKABLE void deleteFile(const QString& path);
    Q_INVOKABLE void move(const QString& fromPath, const QString& toPath);
    Q_INVOKABLE void rename(const QString& fromPath, const QString& toPath);
    Q_INVOKABLE void getCurrentAccount();
    Q_INVOKABLE void getSpaceUsage();
    Q_INVOKABLE void getThumbnail(const QString& path, const QString& size = "w128h128");
    Q_INVOKABLE void download(const QString& path);
    Q_INVOKABLE void upload(const QString& localPath, const QString& remotePath);

    Q_INVOKABLE const QVariantList& getSelected() const;
    Q_INVOKABLE void setSelected(const QVariantList& selected);
    Q_INVOKABLE void select(const QVariantMap& file);
    Q_INVOKABLE void unselectAll();

    Q_INVOKABLE const QVariantList& getDownloads() const;
    Q_INVOKABLE const QVariantList& getUploads() const;

    Q_SIGNALS:
        void listFolderLoaded(const QString& path, const QVariantList& files, const QString& cursor, const bool& hasMore);
        void listFolderContinueLoaded(const QVariantList& files, const QString& prevCursor, const QString& cursor, const bool& hasMore);
        void folderCreated(const QVariantMap& folder);
        void fileDeleted(const QVariantMap& file);
        void moved(const QVariantMap& file);
        void renamed(const QVariantMap& file);
        void currentAccountLoaded(Account* account);
        void spaceUsageLoaded(const QVariantMap& spaceUsage);
        void thumbnailLoaded(const QString& path, const QString& localPath);
        void downloadsChanged(const QVariantList& downloads);
        void downloaded(const QString& path, const QString& localPath);
        void downloadProgress(const QString& path, qint64 loaded, qint64 total);
        void downloadStarted(const QString& path);
        void uploadsChanged(const QVariantList& uploads);
        void uploadStarted(const QString& remotePath);
        void uploaded(const QVariantMap& file);
        void uploadProgress(const QString& remotePath, qint64 loaded, qint64 total);

        void selectedChanged(const QVariantList& selected);

private slots:
    void onListFolderLoaded(const QString& path, QList<QDropboxFile*>& files, const QString& cursor, const bool& hasMore);
    void onListFolderContinueLoaded(QList<QDropboxFile*>& files, const QString& prevCursor, const QString& cursor, const bool& hasMore);
    void onFolderCreated(QDropboxFile* folder);
    void onFileDeleted(QDropboxFile* file);
    void onMoved(QDropboxFile* file);
    void onRenamed(QDropboxFile* file);
    void onSpaceUsageLoaded(QDropboxSpaceUsage* spaceUsage);
    void onThumbnailLoaded(const QString& path, const QString& size, QImage* thumbnail);
    void onDownloaded(const QString& path, const QString& localPath);
    void onDownloadStarted(const QString& path);
    void onUploaded(QDropboxFile* file);
    void onUploadStarted(const QString& remotePath);

private:
    static Logger logger;

    QDropbox* m_pQDropbox;
    FileUtil* m_pFileUtil;

    QStringList m_pathsList;
    QVariantList m_selected;
    QVariantList m_downloads;
    QVariantList m_uploads;

    SystemToast m_toast;

    void clear(QList<QDropboxFile*>& files);
};

#endif /* QDROPBOXCONTROLLER_HPP_ */
