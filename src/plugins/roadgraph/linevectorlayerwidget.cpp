/***************************************************************************
 *   Copyright (C) 2010 by Sergey Yakushev                                 *
 *   yakushevs@list.ru                                                     *
 *                                                                         *
 *   This is a plugin generated from the QGIS plugin template              *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 ***************************************************************************/
#include "linevectorlayerwidget.h"
#include "linevectorlayersettings.h"

//qt includes
#include <qgscontexthelp.h>
#include <qlabel.h>
#include <qcombobox.h>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <qdialogbuttonbox.h>
#include <qtabwidget.h>
#include <qspinbox.h>
#include <qmessagebox.h>
#include <qlineedit.h>

// Qgis includes
#include "qgsfields.h"
#include "qgsvectorlayer.h"
#include "qgsmaplayercombobox.h"

//standard includes


RgLineVectorLayerSettingsWidget::RgLineVectorLayerSettingsWidget( RgLineVectorLayerSettings *s, QWidget* parent )
    : QWidget( parent )
{
  // create base widgets;
  QTabWidget *tab = new QTabWidget( this );
  QVBoxLayout *v = new QVBoxLayout( this );
  v->addWidget( tab );

  // transportation layer
  QFrame *frame = new QFrame( this );
  tab->addTab( frame, tr( "Transportation layer" ) );
  v = new QVBoxLayout( frame );
  QLabel *l = new QLabel( tr( "Layer" ), frame );
  mcbLayers = new QgsMapLayerComboBox( frame );
  mcbLayers->setFilters( QgsMapLayerProxyModel::LineLayer );
  QHBoxLayout *h = new QHBoxLayout();

  h->addWidget( l );
  h->addWidget( mcbLayers );
  v->addLayout( h );

  h = new QHBoxLayout();
  l = new QLabel( tr( "Direction field" ), frame );
  mcbDirection = new QComboBox( frame );
  h->addWidget( l );
  h->addWidget( mcbDirection );
  v->addLayout( h );

  h = new QHBoxLayout();
  h->addWidget( new QLabel( tr( "Value for forward direction" ), frame ) );
  mleFirstPointToLastPointDirection = new QLineEdit( s->mFirstPointToLastPointDirectionVal, frame );
  h->addWidget( mleFirstPointToLastPointDirection );
  v->addLayout( h );

  h = new QHBoxLayout();
  h->addWidget( new QLabel( tr( "Value for reverse direction" ), frame ) );
  mleLastPointToFirstPointDirection = new QLineEdit( s->mLastPointToFirstPointDirectionVal, frame );
  h->addWidget( mleLastPointToFirstPointDirection );
  v->addLayout( h );

  h = new QHBoxLayout();
  h->addWidget( new QLabel( tr( "Value two-way direction" ), frame ) );
  mleBothDirection = new QLineEdit( s->mBothDirectionVal, frame );
  h->addWidget( mleBothDirection );
  v->addLayout( h );

  h = new QHBoxLayout();
  l = new QLabel( tr( "Speed field" ), frame );
  mcbSpeed = new QComboBox( frame );
  h->addWidget( l );
  h->addWidget( mcbSpeed );
  mcbUnitOfSpeed = new QComboBox( this );
  h->addWidget( mcbUnitOfSpeed );

  mcbUnitOfSpeed->insertItem( 0, tr( "km/h" ) );
  mcbUnitOfSpeed->insertItem( 0, tr( "m/s" ) );

  v->addLayout( h );

  frame = new QFrame( tab );
  tab->addTab( frame, tr( "Default settings" ) );
  v = new QVBoxLayout( frame );
  h = new QHBoxLayout();
  l = new QLabel( tr( "Direction" ), frame );
  mcbDirectionDefault = new QComboBox( frame );
  mcbDirectionDefault->insertItem( 0, tr( "Two-way direction" ) );
  mcbDirectionDefault->insertItem( 1, tr( "Forward direction" ) );
  mcbDirectionDefault->insertItem( 2, tr( "Reverse direction" ) );
  connect( mcbLayers, SIGNAL( currentIndexChanged( int ) ), this, SLOT( on_mcbLayers_selectItem() ) );

  h->addWidget( l );
  h->addWidget( mcbDirectionDefault );
  v->addLayout( h );

  h = new QHBoxLayout();
  l = new QLabel( tr( "Cost" ), frame );
  h->addWidget( l );
  l = new QLabel( tr( "Line lengths" ), frame );
  h->addWidget( l );
  v->addLayout( h );

  h = new QHBoxLayout();
  l = new QLabel( tr( "Speed" ), frame );
  msbSpeedDefault = new QSpinBox( frame );
  msbSpeedDefault->setMinimum( 1 );
  msbSpeedDefault->setMaximum( 10000000 );
  h->addWidget( l );
  h->addWidget( msbSpeedDefault );
  v->addLayout( h );

  //sets current settings
  msbSpeedDefault->setValue( static_cast<int>( s->mDefaultSpeed ) );

  int idx = mcbLayers->findText( s->mLayerName );
  if ( idx != -1 )
  {
    mcbLayers->setCurrentIndex( idx );
  }
  on_mcbLayers_selectItem();

  idx = mcbDirection->findText( s->mDirection );
  if ( idx != -1 )
    mcbDirection->setCurrentIndex( idx );

  idx = mcbSpeed->findText( s->mSpeed );
  if ( idx != -1 )
    mcbSpeed->setCurrentIndex( idx );


  switch ( s->mDefaultDirection )
  {
    case QgsVectorLayerDirector::Direction::DirectionBoth:
      mcbDirectionDefault->setCurrentIndex( 0 );
      break;
    case QgsVectorLayerDirector::Direction::DirectionForward:
      mcbDirectionDefault->setCurrentIndex( 1 );
      break;
    case QgsVectorLayerDirector::Direction::DirectionBackward:
      mcbDirectionDefault->setCurrentIndex( 2 );
      break;
  }
  if ( s->mSpeedUnitName == QLatin1String( "km/h" ) )
    mcbUnitOfSpeed->setCurrentIndex( 1 );
  else if ( s->mSpeedUnitName == QLatin1String( "m/s" ) )
    mcbUnitOfSpeed->setCurrentIndex( 0 );

}

QgsVectorLayer* RgLineVectorLayerSettingsWidget::selectedLayer()
{
  return dynamic_cast< QgsVectorLayer* >( mcbLayers->currentLayer() );
}

void RgLineVectorLayerSettingsWidget::on_mcbLayers_selectItem()
{
  mcbDirection->clear();
  mcbSpeed->clear();

  mcbDirection->insertItem( 0, tr( "Always use default" ) );
  mcbSpeed->insertItem( 0, tr( "Always use default" ) );

  QgsVectorLayer* vl = selectedLayer();
  if ( !vl )
    return;

  Q_FOREACH ( const QgsField& currentField, vl->fields() )
  {
    QVariant currentType = currentField.type();
    if ( currentType == QVariant::Int || currentType == QVariant::LongLong ||
         currentType == QVariant::String )
    {
      mcbDirection->insertItem( 1, currentField.name() );
    }
    if ( currentType == QVariant::Int || currentType == QVariant::LongLong ||
         currentType == QVariant::Double )
    {
      mcbSpeed->insertItem( 1, currentField.name() );
    }
  }
}
