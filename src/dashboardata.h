#ifndef DASHBOARDDATA_H
#define DASHBOARDDATA_H

#include <QMap>
#include <QString>
#include <cstdint>

struct RepositoryMetrics
{
  uint64_t rawSize{ 0 };          // Tamanho original (Original size)
  uint64_t compressedSize{ 0 };   // Este backup (Compressed size)
  uint64_t deduplicatedSize{ 0 }; // Valor deduplicado (Deduplicated size)

  // Helper para calcular a eficiência individual
  double efficiency() const
  {
    return deduplicatedSize > 0
             ? static_cast<double>(rawSize) / deduplicatedSize
             : 0.0;
  }
};

// O QMap vincula o nome do repositório aos seus dados vindos do 'borg --info'
using RepoDataMap = QMap<QString, RepositoryMetrics>;

#endif // DASHBOARDDATA_H