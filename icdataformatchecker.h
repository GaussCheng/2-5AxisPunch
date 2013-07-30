#ifndef ICDATAFORMATCHECKER_H
#define ICDATAFORMATCHECKER_H

#include <QString>
class ICDataFormatChecker
{
public:
    enum CompareWay
    {
        kCompareLargger,
        kCompareSmaller,
        kCompareEqual,
        kCompareLarggerOrEqual,
        kCompareSmallerOrEqual
    };
    ICDataFormatChecker();

    QString Content() const { return content_;}
    void SetContent(const QString& content) { content_ = content;}

    bool Check(const QString& dataStream){return CheckImpl(dataStream);}
    bool Check(){return Check(Content());}
    bool CheckRowCount(const QString& dataStream, int rowCount, CompareWay way);
    bool CheckRowCount(int rowCount, CompareWay way) { return CheckRowCount(Content(),
                                                                            rowCount,
                                                                            way);}

    bool CheckColumCount(const QString& dataStream, int row, const QString &spliter, int colCount, CompareWay way);
    bool CheckColumCount(int row, const QString &spliter, int colCount, CompareWay way) { return CheckColumCount(Content(),
                                                                                                                 row,
                                                                                                                 spliter,
                                                                                                                 colCount,
                                                                                                                 way);}

    bool CheckDataVal(const QString& dataStream,
                      int row,
                      int col,
                      const QString& spliter,
                      const QString& val);
    bool CheckDataVal(int row,
                      int col,
                      const QString& spliter,
                      const QString& val)
    {
        return CheckDataVal(Content(),
                            row,
                            col,
                            spliter,
                            val);
    }
protected:
    virtual bool CheckImpl(const QString& dataStream) { Q_UNUSED(dataStream);return false;}

private:
    QString content_;
};

#endif // ICDATAFORMATCHECKER_H
