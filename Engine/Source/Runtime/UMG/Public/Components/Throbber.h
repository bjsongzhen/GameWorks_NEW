// Copyright 1998-2014 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Throbber.generated.h"

class USlateBrushAsset;

/** A Throbber widget that shows several zooming circles in a row. */
UCLASS(meta=( Category="Misc" ), ClassGroup=UserInterface)
class UMG_API UThrobber : public UWidget
{
	GENERATED_UCLASS_BODY()

public:

	/** Sets how many pieces there are */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetNumberOfPieces(int32 InNumberOfPieces);

	/** Sets whether the pieces animate horizontally. */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetAnimateHorizontally(bool bInAnimateHorizontally);

	/** Sets whether the pieces animate vertically. */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetAnimateVertically(bool bInAnimateVertically);

	/** Sets whether the pieces animate their opacity. */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetAnimateOpacity(bool bInAnimateOpacity);

	/** Sets the image to use for each segment of the throbber. */
	UFUNCTION(BlueprintCallable, Category = "Appearance")
	void SetPieceImage(USlateBrushAsset* InPieceImage);

	// UWidget interface
	virtual void SyncronizeProperties() override;
	// End of UWidget interface

protected:

	/** How many pieces there are */
	UPROPERTY(EditDefaultsOnly, Category=Appearance, meta=(ClampMin = "1", UIMin = "1", UIMax = "8"))
	int32 NumberOfPieces;

	/** Should the pieces animate horizontally? */
	UPROPERTY(EditDefaultsOnly, Category=Appearance)
	bool bAnimateHorizontally;

	/** Should the pieces animate vertically? */
	UPROPERTY(EditDefaultsOnly, Category=Appearance)
	bool bAnimateVertically;

	/** Should the pieces animate their opacity? */
	UPROPERTY(EditDefaultsOnly, Category=Appearance)
	bool bAnimateOpacity;

	/** Image to use for each segment of the throbber */
	UPROPERTY(EditDefaultsOnly, Category=Appearance, meta=( DisplayThumbnail = "true" ))
	USlateBrushAsset* PieceImage;

protected:
	// UWidget interface
	virtual TSharedRef<SWidget> RebuildWidget() override;
	// End of UWidget interface

private:

	/** Gets the combined value of the animation properties as a single SThrobber::EAnimation value. */
	SThrobber::EAnimation GetAnimation() const;

	/** Gets the brush which should be used to draw the trobber pieces. */
	const FSlateBrush* GetPieceBrush() const;

private:
	/** The Throbber widget managed by this object. */
	TSharedPtr<SThrobber> MyThrobber;
};