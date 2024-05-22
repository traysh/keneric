//    Copyright (C) 2015 Rog131 <samrog131@hotmail.com>
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program; if not, write to the Free Software
//    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#include "keneric.h"

#include <KPluginFactory>
#include <QCryptographicHash>
#include <QDir>
#include <QFile>
#include <QImage>
#include <QMimeDatabase>
#include <QMimeType>
#include <QProcess>
#include <QStandardPaths>

Keneric::Keneric(QObject *parent, const QVariantList &args)
    : KIO::ThumbnailCreator(parent, args) {}

Keneric::~Keneric() {}

// bool Keneric::create(const QString &path, int width, int height, QImage &img)
// {
KIO::ThumbnailResult Keneric::create(const KIO::ThumbnailRequest &request) {
  const auto width = request.targetSize().width();
  const auto height = request.targetSize().height();
  const auto path = request.url().path();
  QImage img;

  // std::cout << "Create Width: " << width << std::endl;
  // std::cout << "Create Height: " << width << std::endl;

  QMimeDatabase db;
  QMimeType mime = db.mimeTypeForFile(path);

  QString kenericDirectory(
      (QStandardPaths::writableLocation(QStandardPaths::GenericCacheLocation) +
       QStringLiteral("/keneric/")));
  QString md5Hash = QString::fromUtf8(
      QCryptographicHash::hash((path.toUtf8()), QCryptographicHash::Md5)
          .toHex());
  QString protoThumbnail(kenericDirectory + md5Hash);

  QDir directory(kenericDirectory);
  if (!directory.exists()) {
    directory.mkpath(QStringLiteral("."));
  }

  QObject *parent = nullptr;
  QString program = QStringLiteral("kenericProcess");
  QStringList arguments;
  arguments << path << mime.name() << protoThumbnail
            << QStringLiteral("%1").arg(width)
            << QStringLiteral("%1").arg(height);

  QProcess *startAction = new QProcess(parent);
  startAction->start(program, arguments);
  startAction->waitForFinished();

  QFile thumbnailFile(protoThumbnail);
  if (thumbnailFile.exists()) {
    QImage previewImage(protoThumbnail);
    previewImage = previewImage.scaled(width, height, Qt::KeepAspectRatio,
                                       Qt::SmoothTransformation);
    img.swap(previewImage);
    QFile::remove(protoThumbnail);
  }

  if (img.isNull()) {
    return KIO::ThumbnailResult::fail();
  }
  return KIO::ThumbnailResult::pass(img);
}

K_PLUGIN_CLASS_WITH_JSON(Keneric, "keneric.json")

#include "keneric.moc"
#include "moc_keneric.cpp"
